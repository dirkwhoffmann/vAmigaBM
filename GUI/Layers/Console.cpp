// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Application.h"

Console::Console(Application &ref) : Layer(ref)
{
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
}

void
Console::awake()
{
    
}

void
Console::respond(const sf::Event &event)
{
    switch (event.type) {
            
        case sf::Event::KeyPressed:
            
            keyPressed(event.key.code);
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
Console::update(u64 frame, sf::Time dt)
{
    Layer::update(frame, dt);
    
    if (frame % 32 == 0) {
        cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,(frame % 64) ? 0x80 : 0));
        isDirty = true;
    }
    
    if (isDirty) {
        updateTexture();
        isDirty = false;
    }
}

void
Console::render()
{
    view.draw(app.window);
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
Console::alphaDidChange()
{
    view.rectangle.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
}

void
Console::setNumRows(isize value)
{
    value = std::min(value, (isize)255);

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
Console::scrollTo(isize line)
{
    isize max = amiga.retroShell.getStorage().size() - 1;
    
    if (line < 0) line = 0;
    if (line > max) line = max;
            
    isDirty = line != vpos;
    vpos = line;
}

void
Console::makeLastLineVisible()
{
    auto &storage = amiga.retroShell.getStorage();
    
    if (!lastLineIsVisible()) {
        scrollTo((isize)storage.size() - numRows);
    }
}

isize
Console::rowOfLastLine()
{
    auto &storage = amiga.retroShell.getStorage();
    
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
    switch (c) {
            
        case '\n':
        case '\b':
        case '\t':
            break;
            
        default:
            
            amiga.retroShell.pressKey(c);
    }

    makeLastLineVisible();
    isDirty = true;
}

void
Console::keyPressed(const sf::Keyboard::Key& key)
{
    bool jumpToLastLine = true;
        
    switch (key) {
            
        case sf::Keyboard::Return:
            amiga.retroShell.pressReturn();
            break;
            
        case sf::Keyboard::Backspace:
            amiga.retroShell.pressBackspace();
            break;

        case sf::Keyboard::Tab:
            amiga.retroShell.pressTab();
            break;

        case sf::Keyboard::Up:
            amiga.retroShell.pressUp();
            break;

        case sf::Keyboard::Down:
            amiga.retroShell.pressDown();
            break;

        case sf::Keyboard::Left:
            amiga.retroShell.pressLeft();
            break;

        case sf::Keyboard::Right:
            amiga.retroShell.pressRight();
            break;
            
        case sf::Keyboard::Home:
            amiga.retroShell.pressHome();
            break;

        case sf::Keyboard::End:
            amiga.retroShell.pressEnd();
            break;

        case sf::Keyboard::PageUp:
            scrollUp(numRows);
            jumpToLastLine = false;
            break;
            
        case sf::Keyboard::PageDown:
            
            scrollDown(numRows);
            jumpToLastLine = false;
            break;
            
        default:
            return;
    }
    
    if (jumpToLastLine) makeLastLineVisible();
    isDirty = true;
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
Console::updateTexture()
{
    auto &storage = amiga.retroShell.getStorage();
    
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
    isize cursorX = hposForCol(amiga.retroShell.cposAbs());
    isize cursorY = vposForRow(rowOfLastLine()) + 3;
    cursor.setPosition(cursorX, cursorY);
    texture.draw(cursor);
}
