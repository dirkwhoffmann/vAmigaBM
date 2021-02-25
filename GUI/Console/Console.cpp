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

Console::Console(Application &ref) : Layer(ref)
{
    // Initialize the text storage
    storage.push_back("");

    // Initialize the input buffer
    input.push_back("");
}

Console::~Console()
{
}

void
Console::init()
{
    Layer::init();
     
    // Get texture dimensions
    auto size = Screen::maxDimensions();
    printf("Console: Allocating %d x %d texture\n", size.x, size.y);
    
    // Create the render texture
    if (!texture.create (size.x, size.y)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    
    // Embed texture in an image view
    view.init(texture.getTexture());
    
    // Initialize font parameters
    sf::Font& font = app.assets.get(FontID::console);
    glyphWidth = font.getGlyph(32, fontSize, false).advance;
            
    // Initialize cursor
    cursor.setSize(sf::Vector2f(glyphWidth + 2, fontSize + 3));
    cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,0x80));

    // Print intro message
    *this << "Retro shell 0.1, ";
    *this << "Dirk W. Hoffmann, ";
    *this << __DATE__ << " " << __TIME__ << "." << '\n';
    *this << "Linked to vAmiga core ";
    *this << V_MAJOR << '.' << V_MINOR << '.' << V_SUBMINOR << '.' << '\n' << '\n';

    printHelp();
    *this << '\n';
    printPrompt();
}

void
Console::configure()
{
    
}

void
Console::handle(const sf::Event &event)
{
    switch (event.type) {
            
        case sf::Event::KeyPressed:
            
            keyPressed(event.key.code);
            break;
            
        case sf::Event::KeyReleased:
            
            keyReleased(event.key.code);
            break;
            
        case sf::Event::TextEntered:
            
            if (event.text.unicode < 128) {
                type(static_cast<char>(event.text.unicode));
            }
            break;
            
        case sf::Event::MouseWheelScrolled:

            if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                scroll(event.mouseWheelScroll.delta);
            }
            break;
            
        default:
            break;
    }
}

void
Console::render()
{
    view.rectangle.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
    
    if (isVisible()) {
        
        if (isDirty) {
            updateTexture();
            isDirty = false;
        }
        view.draw(app.window); 
    }
}

void
Console::resize(float width, float height)
{
    printf("Console: resize(%f,%f)\n", width, height);
    
    setNumRows((height - 2 * pady) / (fontSize + lineSkip));
    setNumCols((width - 2 * padx) / glyphWidth);
    
    printf("Rows: %zd, Columns: %zd\n", numRows, numCols);
}

void
Console::setNumRows(isize value)
{
    value = MIN(value, 255);

    numRows = value;
    isDirty = true;
}

void
Console::setNumCols(isize value)
{
    numCols = value;
    isDirty = true;
}

void
Console::clear()
{
    scrollTo(65536);
}

void
Console::printHelp()
{
    *this << "Press 'TAB' twice for help." << '\n';
}

void
Console::printPrompt()
{
    // Finish the current line (if neccessary)
    if (!lastLine().empty()) *this << '\n';

    // Print the prompt
    *this << prompt;
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
    
    shorten();
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
    
    shorten();
    return *this;
}

Console&
Console::operator<<(int value)
{
    *this << std::to_string(value);
    return *this;
}

Console&
Console::operator<<(long value)
{
    *this << std::to_string(value);
    return *this;
}

void
Console::shorten()
{
    while (storage.size() > 600) {
        
        storage.erase(storage.begin());
        scrollUp(1);
    }
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
        printf("%zd: %s\n", i, input[i].c_str());
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

bool
Console::lastLineIsVisible()
{
    return rowOfLastLine() >= 0 && rowOfLastLine() < numRows;
}

void
Console::type(char c)
{
    static isize doubleTab = 0;
    
    switch (c) {
            
        case '\n':

            *this << '\n';
                        
            // Print a help message if no input is given
            if (input[ipos].empty()) {
                printHelp();
                printPrompt();
                break;
            }

            // Add the current input line to the user input history
            input[input.size() - 1] = input[ipos];

            // Create a new input line
            input.push_back("");
            ipos = (isize)input.size() - 1;

            // Execute the command
            exec(input[ipos - 1]);
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
                interpreter.help(input[ipos]);
                
                // Repeat the old input string
                *this << string(prompt) << input[ipos];
                
            } else {
                
                interpreter.autoComplete(input[ipos]);
                cpos = (isize)input[ipos].length();
                replace(input[ipos]);
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

bool
Console::exec(const string &command, bool verbose)
{
    bool success = false;
    
    // Print the command string if requested
    if (verbose) *this << command << '\n';
        
    printf("Command: %s\n", command.c_str());
    
    try {
        
        // Hand the command over to the intepreter
        interpreter.exec(command);
        success = true;
               
    } catch (TooFewArgumentsError &err) {
        *this << err.what() << ": Too few arguments";
        *this << '\n';
        
    } catch (TooManyArgumentsError &err) {
        *this << err.what() << ": Too many arguments";
        *this << '\n';
        
    } catch (ParseEnumError &err) {
        *this << err.token << ": Invalid key" << '\n';
        *this << "Expected: " << err.expected << '\n';
        
    } catch (ParseBoolError &err) {
        *this << "Invalid key" << '\n';
        *this << "Expected: true or false" << '\n';

    } catch (ParseError &err) {
        *this << err.what() << ": Syntax error";
        *this << '\n';
        // *this << "Expected: " << err.what() << '\n';
        
    } catch (ConfigUnsupportedError) {
        *this << "This option is not yet supported.";
        *this << '\n';
        
    } catch (ConfigLockedError &err) {
        *this << "This option is locked because the Amiga is powered on.";
        *this << '\n';
        
    } catch (ConfigArgError &err) {
        *this << "Error: Invalid argument. Expected: " << err.what();
        *this << '\n';
        
    } catch (ConfigFileReadError &err) {
        *this << "Error: Unable to read file " << err.what();
        *this << '\n';
        
    } catch (VAError &err) {
        *this << err.what();
        *this << '\n';
    }
    
    // Print a new prompt
    // printf("Command: %s\n", command.c_str());
    // *this << string(prompt);
    printPrompt();
    cpos = 0;
    
    return success;
}

void
Console::exec(std::istream &stream)
{
    isize line = 0;
    string command;
        
    while(std::getline(stream, command)) {

        line++;
        printf("Line %zd: %s\n", line, command.c_str());

        // Skip empty lines
        if (command == "") continue;

        // Skip comments
        if (command.substr(0,1) == "#") continue;
        
        // Execute the command
        if (!exec(command, true)) {
            throw Exception(command, line);
        }
    }
}

void
Console::updateTexture()
{
    texture.clear(sf::Color(0x21,0x21,0x21,0xD0));
    
    // Instantiate missing text objects
    for (isize i = text.size(); i < numRows; i++) {
        text.push_back(sf::Text());
        sf::Text &ref = text.back();
        ref.setFont(app.assets.get(FontID::console));
        ref.setCharacterSize(fontSize);
        ref.setFillColor(sf::Color::White);
        ref.setPosition(hposForCol(0), vposForRow(i));
    }
    
    // Draw all text rows
    for (isize i = 0; i < numRows; i++) {
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
