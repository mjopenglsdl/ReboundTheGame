//
// Copyright (c) 2016-2018 João Baptista de Paula e Silva.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//


#include "UIButtonCommons.hpp"

#include "resources/ResourceManager.hpp"
#include "resources/FontHandler.hpp"
#include "language/LocalizationManager.hpp"
#include "language/convenienceConfigText.hpp"
#include "drawables/Sprite.hpp"
#include "drawables/SegmentedSprite.hpp"

void createCommonTextualButton(UIButton& button, ResourceManager& rm, LocalizationManager& lm,
    std::string activeResourceName, std::string pressedResourceName, sf::FloatRect centerRect,
    sf::FloatRect destRect, LangID captionString, size_t captionSize, sf::Color textColor,
    float outlineThickness, sf::Color outlineColor, sf::Vector2f captionDisplacement,
    TextDrawable::Alignment wordAlignment)
{
    sf::Vector2f destCenter(destRect.width/2, destRect.height/2);
    
    auto activeSprite = std::make_unique<SegmentedSprite>(rm.load<sf::Texture>(activeResourceName));
    activeSprite->setCenterRect(centerRect);
    activeSprite->setDestinationRect(destRect);
    activeSprite->setAnchorPoint(destCenter);
    
    auto pressedSprite = std::make_unique<SegmentedSprite>(rm.load<sf::Texture>(pressedResourceName));
    pressedSprite->setCenterRect(centerRect);
    pressedSprite->setDestinationRect(destRect);
    pressedSprite->setAnchorPoint(destCenter);
    
    auto caption = std::make_unique<TextDrawable>(rm.load<FontHandler>(lm.getFontName()));
    if (!captionString.empty()) caption->setString(lm.getString(captionString));
    caption->setFontSize(captionSize);
    caption->setDefaultColor(textColor);
    caption->setOutlineThickness(outlineThickness);
    caption->setDefaultOutlineColor(outlineColor);
    caption->setHorizontalAnchor(TextDrawable::HorAnchor::Center);
    caption->setVerticalAnchor(TextDrawable::VertAnchor::Center);
    caption->setWordWrappingWidth(destRect.width - 2 * captionDisplacement.x);
    caption->setWordAlignment(wordAlignment);
    configTextDrawable(*caption, lm);
    caption->buildGeometry();
    
    button.setActiveSprite(std::move(activeSprite));
    button.setPressedSprite(std::move(pressedSprite));
    button.setCaption(std::move(caption));
    button.setCaptionDisplacement(captionDisplacement);
    button.autoComputeBounds();
}

void createCommonGraphicsButton(UIButton& button, ResourceManager& rm, std::string normalResourceName,
    std::string activeResourceName, std::string pressedResourceName)
{
    
}
