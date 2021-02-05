// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

const std::string& Console::prompt = string("vAmiga\% ");

Console::Console(Application &ref) : app(ref)
{
}

Console::~Console()
{
}

bool
Console::init()
{
    sf::Font& font = Assets::get(FontID::console);

    // Initialize the text storage
    storage.push_back("");

    // Initialize the input buffer
    input.push_back("");

    *this << "Retro shell 0.1, ";
    *this << "Dirk W. Hoffmann, ";
    *this << __DATE__ << " " << __TIME__ << "." << '\n';
    *this << "Linked to vAmiga core " << V_MAJOR << '.' << V_MINOR << '.' << V_SUBMINOR;
    *this << '.' << '\n' << '\n';
    app.interpreter.help();
    
    *this << prompt;
        
    // Initialize the cursor
    glyphWidth = font.getGlyph(32, fontSize, false).advance;
    cursor.setSize(sf::Vector2f(glyphWidth + 2, fontSize + 3));
    cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,0x80));
    
    // Initialize the render target
    if (!texture.create(width, height)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    drawable.setTextureRect(sf::IntRect(0, height, width, -height));
    drawable.setTexture(&texture.getTexture());
            
    auto winSize = app.window.getSize();
    sf::Vector2f size = { (float)winSize.x, (float)winSize.y };
    drawable.setSize(size);
 
    // Initialize render items
    for (int i = 0; i < numRows; i++) {
        
        text[i].setFont(font);
        text[i].setString("");
        text[i].setCharacterSize(fontSize);
        text[i].setFillColor(sf::Color::White);
        text[i].setPosition(hposForCol(0), vposForRow(i));
    }
    
    return true;
}

void
Console::clear()
{
    printf("::clear\n");
    scrollTo(65536);
}

Console&
Console::operator<<(char value)
{
    if (value == '\n') {

        // Newline (appends an empty line)
        storage.push_back("");

    } else if (value == '\r') {

        // Carriage return (clears the current line)
        storage.back() = "";
        
    } else {
        
        // Add a single character
        if (storage.back().length() >= numCols) storage.push_back("");
        storage.back() += value;
    }
    
    // Shorten the text storage if it has grown too large
    if (storage.size() > 100) storage.erase(storage.begin());
        
    return *this;
}

Console&
Console::operator<<(const std::string& text)
{
    size_t remaining = numCols - storage.back().length();
    
    // Split the string if it is too large
    if (text.length() > remaining) {
        *this << text.substr(0, remaining) << '\n' << text.substr(remaining);
    } else {
        storage.back() += text;
    }
    
    // Shorten the text storage if it has grown too large
    if (storage.size() > 100) storage.erase(storage.begin());

    return *this;
}

Console&
Console::operator<<(int value)
{
    *this << std::to_string(value);
    return *this;
}

void
Console::tab(int hpos)
{
    int delta = hpos - (int)storage.back().length();
    for (int i = 0; i < delta; i++) {
        *this << ' ';
    }
}


void
Console::replace(const string& text, const string& prefix)
{
    
    storage.back() = prefix + text.substr(0, numCols);
}

void
Console::list()
{
    for (isize i = 0; i < input.size(); i++) {
        printf("%ld: %s\n", i, input[i].c_str());
    }
}

void
Console::scrollTo(isize line)
{
    line = std::clamp(line, (isize)0, (isize)storage.size() - 1);
        
    isDirty = line != vpos;
    vpos = line;
}

void
Console::makeLastLineVisible()
{
    if (!lastLineIsVisible()) {
        scrollTo((int)storage.size() - numRows);
    }
}

isize
Console::rowOfLastLine()
{
    return (isize)storage.size() - vpos - 1;
}

void
Console::type(char c)
{
    static isize doubleTab = 0;
    
    switch (c) {
            
        case '\n':

            *this << '\n';
            
            // Execute the command
            app.interpreter.exec(input[ipos]);
            
            // Add the command to the user input history
            input[input.size() - 1] = input[ipos];

            // Print a new prompt
            input.push_back("");
            ipos = (int)input.size() - 1;
            cpos = 0;
            
            // Print a new prompt
            *this << string(prompt);
            break;
            
        case '\b':
            
            if (cpos > 0) {
                input[ipos].erase(input[ipos].begin() + --cpos);
            }
            *this << '\r' << string(prompt) << input[ipos];
            break;
            
        case '\t':
            
            if (doubleTab) {
                
                *this << '\n';

                // Print the instructions for this command
                app.interpreter.execSyntax(input[ipos]);
                
                // Repeat the old input string
                *this << string(prompt) << input[ipos];
                
            } else {
                
                string completion = app.interpreter.autoComplete(input[ipos]);
                if (completion.size() > input[ipos].size()) {
                    input[ipos] = completion;
                    cpos = (isize)input[ipos].length();
                    replace(input[ipos]);
                }
            }
            
            doubleTab = 2;
            break;
            
        default:
            
            if (input[ipos].length() < numCols - (int)prompt.length() - 1) {
                
                input[ipos].insert(input[ipos].begin() + cpos++, c);
            }
            *this << '\r' << string(prompt) << input[ipos];
    }

    if (doubleTab > 0) doubleTab--;
    makeLastLineVisible();
    isDirty = true;
}

void
Console::keyPressed(const sf::Keyboard::Key& key)
{
    switch (key) {
            
        case sf::Keyboard::Up:

            if (ipos > 0) {
                ipos--;
                cpos = (isize)input[ipos].size();

                replace(input[ipos]);
            }
            makeLastLineVisible();
            break;

        case sf::Keyboard::Down:

            if (ipos < input.size() - 1) {
                ipos++;
                cpos = (isize)input[ipos].size();
                
                replace(input[ipos]);
            }
            makeLastLineVisible();
            break;
            
        case sf::Keyboard::Left:

            if (cpos > 0) {
                cpos--;
            }
            makeLastLineVisible();
            break;
            
        case sf::Keyboard::Right:
            
            if (cpos < input[ipos].length()) {
                cpos++;
            }
            makeLastLineVisible();
            break;
            
        case sf::Keyboard::Home:
            
            cpos = 0;
            makeLastLineVisible();
            break;

        case sf::Keyboard::End:
            
            cpos = (isize)input[ipos].length();
            makeLastLineVisible();
            break;

        case sf::Keyboard::PageUp:
            
            scrollUp(numRows);
            break;
            
        case sf::Keyboard::PageDown:
            
            scrollDown(numRows);
            break;
            
        default:
            return;
    }
    
    isDirty = true;
}

void
Console::keyReleased(const sf::Keyboard::Key& key)
{
    /*
    printf("keyReleased\n");

    switch (key) {
            
        case sf::Keyboard::LControl:
            break;

        default:
            break;
    }
    */
}
      
void
Console::scroll(float delta)
{    
    static float vpos = 0.0;
    
    float newpos = vpos + delta;
    isize dy = (isize)abs(newpos);
    
    if (dy >= 1) {
        if (newpos > 0) { scrollUp(dy); newpos -= dy; }
        if (newpos < 0) { scrollDown(dy); newpos += dy; }
    }
}

void
Console::render(sf::RenderWindow &window)
{
    if (isAnimating()) {
        if (isOpening()) alpha = std::min(targetAlpha, alpha + 24);
        if (isClosing()) alpha = std::max(targetAlpha, alpha - 24);
        drawable.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
    }
        
    if (isVisible()) {
        
        if (isDirty) {
            updateTexture();
            isDirty = false;
        }
        window.draw(drawable);
    }
}

void
Console::updateTexture()
{
    texture.clear(sf::Color(0x21,0x21,0x21,0xD0));
    
    for (int i = 0; i < numRows; i++) {
        
        if (vpos + i < storage.size()) {
            text[i].setString(storage[vpos + i]);
        } else {
            text[i].setString("");
        }
        texture.draw(text[i]);
    }
    
    // Draw cursor
    isize cursorX = hposForCol(cpos + (int)prompt.length());
    isize cursorY = vposForRow(rowOfLastLine()) + 3;
    cursor.setPosition(cursorX, cursorY);
    texture.draw(cursor);
}
