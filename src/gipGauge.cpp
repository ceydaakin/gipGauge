/*
 * gipGauge.cpp
 *
 *  Created on: Mar 11, 2023
 *      Author: Noyan Culum
 *      Created by: kayra
 */

#include "gipGauge.h"
#include "gRenderer.h"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

gipGauge::gipGauge() {
    initializeDefaults();
}

gipGauge::~gipGauge() {
}

void gipGauge::initializeDefaults() {
    // Core values
    currentvalue = 0.0f;
    targetvalue = 0.0f;
    minvalue = 0.0f;
    maxvalue = 100.0f;
    
    // Visual settings
    gaugetype = GAUGE_CIRCULAR;
    gaugestyle = STYLE_MODERN;
    title = "Gauge";
    unit = "";

    // Colors
    backgroundcolor = gColor(0.2f, 0.2f, 0.2f, 1.0f);  // Dark gray
    needlecolor = gColor(1.0f, 0.3f, 0.3f, 1.0f);      // Red
    tickcolor = gColor(0.8f, 0.8f, 0.8f, 1.0f);        // Light gray
    valuetextcolor = gColor(1.0f, 1.0f, 1.0f, 1.0f);   // White
    titlecolor = gColor(0.9f, 0.9f, 0.9f, 1.0f);       // Light gray
    dangerzonecolor = gColor(1.0f, 0.2f, 0.2f, 0.3f);   // Semi-transparent red
    warningzonecolor = gColor(1.0f, 0.8f, 0.0f, 0.3f);  // Semi-transparent yellow
    safezonecolor = gColor(0.2f, 1.0f, 0.2f, 0.3f);     // Semi-transparent green

    // Zone initialization
    dangerzone = {80.0f, 100.0f, dangerzonecolor, false};
    warningzone = {60.0f, 80.0f, warningzonecolor, false};
    safezone = {0.0f, 60.0f, safezonecolor, false};

    // Animation
    animationenabled = true;
    animationspeed = 5.0f;  // Speed factor for smooth transitions
    
    // Ticks
    majortickcount = 10;
    minortickcount = 5;  // Minor ticks between each major tick
    showticklabels = true;

    // Display options
    showvalue = true;
    showtitle = true;
    showneedle = true;

    // Needle properties
    needlelength = 0.8f;  // Relative to radius
    needlewidth = 3.0f;   // Pixels

    // Initialize drawing dimensions
    gaugex = gaugey = 0;
    gaugewidth = gaugeheight = 100;
    centerx = centery = 50.0f;
    radius = 40.0f;
}

void gipGauge::draw(int x, int y, int w, int h) {
    gaugex = x;
    gaugey = y;
    gaugewidth = w;
    gaugeheight = h;
    
    // Calculate center and radius based on gauge type
    switch(gaugetype) {
        case GAUGE_CIRCULAR:
            centerx = x + w * 0.5f;
            centery = y + h * 0.5f;
            radius = std::min(w, h) * 0.4f;
            drawCircularGauge();
            break;
        case GAUGE_SEMICIRCLE:
            centerx = x + w * 0.5f;
            centery = y + h * 0.8f;
            radius = std::min(w, h * 1.25f) * 0.4f;
            drawSemicircleGauge();
            break;
        case GAUGE_LINEAR_HORIZONTAL:
            drawLinearGaugeHorizontal();
            break;
        case GAUGE_LINEAR_VERTICAL:
            drawLinearGaugeVertical();
            break;
    }
    
    update();
}

void gipGauge::update() {
    updateAnimation();
}

void gipGauge::updateAnimation() {
    if (!animationenabled) {
        currentvalue = targetvalue;
        return;
    }
    
    float difference = targetvalue - currentvalue;
    if (std::abs(difference) > 0.001f) {
        float deltaTime = 1.0f / 60.0f;  // Assuming 60 FPS
        float lerpFactor = animationspeed * deltaTime;
        currentvalue = lerp(currentvalue, targetvalue, std::min(lerpFactor, 1.0f));
    } else {
        currentvalue = targetvalue;
    }
}

float gipGauge::lerp(float start, float end, float t) {
    return start + t * (end - start);
}

void gipGauge::drawCircularGauge() {
    drawBackground();
    drawZones();
    drawTicks();
    if (showneedle) drawNeedle();
    if (showvalue) drawValueText();
    if (showtitle) drawTitle();
}

void gipGauge::drawSemicircleGauge() {
    drawBackground();
    drawZones();
    drawTicks();
    if (showneedle) drawNeedle();
    if (showvalue) drawValueText();
    if (showtitle) drawTitle();
}

void gipGauge::drawLinearGaugeHorizontal() {
    // Draw background rectangle
    renderer->setColor(backgroundcolor);
    renderer->drawRectangle(gaugex, gaugey, gaugewidth, gaugeheight, true);
    
    // Draw zones
    drawZones();
    
    // Draw value bar
    float valueRatio = (currentvalue - minvalue) / (maxvalue - minvalue);
    valueRatio = std::max(0.0f, std::min(1.0f, valueRatio));
    
    gColor valueColor = getZoneColorForValue(currentvalue);
    if (valueColor.r == 0 && valueColor.g == 0 && valueColor.b == 0) {
        valueColor = needlecolor;
    }
    
    renderer->setColor(valueColor);
    int barWidth = static_cast<int>(gaugewidth * valueRatio);
    renderer->drawRectangle(gaugex, gaugey, barWidth, gaugeheight, true);
    
    // Draw border
    renderer->setColor(tickcolor);
    renderer->drawRectangle(gaugex, gaugey, gaugewidth, gaugeheight, false);
    
    if (showvalue) drawValueText();
    if (showtitle) drawTitle();
}

void gipGauge::drawLinearGaugeVertical() {
    // Draw background rectangle
    renderer->setColor(backgroundcolor);
    renderer->drawRectangle(gaugex, gaugey, gaugewidth, gaugeheight, true);
    
    // Draw zones
    drawZones();
    
    // Draw value bar
    float valueRatio = (currentvalue - minvalue) / (maxvalue - minvalue);
    valueRatio = std::max(0.0f, std::min(1.0f, valueRatio));
    
    gColor valueColor = getZoneColorForValue(currentvalue);
    if (valueColor.r == 0 && valueColor.g == 0 && valueColor.b == 0) {
        valueColor = needlecolor;
    }
    
    renderer->setColor(valueColor);
    int barHeight = static_cast<int>(gaugeheight * valueRatio);
    renderer->drawRectangle(gaugex, gaugey + gaugeheight - barHeight, gaugewidth, barHeight, true);
    
    // Draw border
    renderer->setColor(tickcolor);
    renderer->drawRectangle(gaugex, gaugey, gaugewidth, gaugeheight, false);
    
    if (showvalue) drawValueText();
    if (showtitle) drawTitle();
}

void gipGauge::drawBackground() {
    renderer->setColor(backgroundcolor);
    if (gaugetype == GAUGE_SEMICIRCLE) {
        renderer->drawArc(centerx, centery, radius, 180.0f, 360.0f, true, 64);
    } else {
        renderer->drawCircle(centerx, centery, radius, true, 64);
    }
}

void gipGauge::drawZones() {
    if (gaugetype == GAUGE_LINEAR_HORIZONTAL) {
        if (safezone.enabled) {
            float startRatio = (safezone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (safezone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(safezone.color);
            int startX = gaugex + static_cast<int>(gaugewidth * startRatio);
            int width = static_cast<int>(gaugewidth * (endRatio - startRatio));
            renderer->drawRectangle(startX, gaugey, width, gaugeheight, true);
        }
        
        if (warningzone.enabled) {
            float startRatio = (warningzone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (warningzone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(warningzone.color);
            int startX = gaugex + static_cast<int>(gaugewidth * startRatio);
            int width = static_cast<int>(gaugewidth * (endRatio - startRatio));
            renderer->drawRectangle(startX, gaugey, width, gaugeheight, true);
        }
        
        if (dangerzone.enabled) {
            float startRatio = (dangerzone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (dangerzone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(dangerzone.color);
            int startX = gaugex + static_cast<int>(gaugewidth * startRatio);
            int width = static_cast<int>(gaugewidth * (endRatio - startRatio));
            renderer->drawRectangle(startX, gaugey, width, gaugeheight, true);
        }
    } else if (gaugetype == GAUGE_LINEAR_VERTICAL) {
        if (safezone.enabled) {
            float startRatio = (safezone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (safezone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(safezone.color);
            int startY = gaugey + static_cast<int>(gaugeheight * (1.0f - endRatio));
            int height = static_cast<int>(gaugeheight * (endRatio - startRatio));
            renderer->drawRectangle(gaugex, startY, gaugewidth, height, true);
        }
        
        if (warningzone.enabled) {
            float startRatio = (warningzone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (warningzone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(warningzone.color);
            int startY = gaugey + static_cast<int>(gaugeheight * (1.0f - endRatio));
            int height = static_cast<int>(gaugeheight * (endRatio - startRatio));
            renderer->drawRectangle(gaugex, startY, gaugewidth, height, true);
        }
        
        if (dangerzone.enabled) {
            float startRatio = (dangerzone.startValue - minvalue) / (maxvalue - minvalue);
            float endRatio = (dangerzone.endValue - minvalue) / (maxvalue - minvalue);
            startRatio = std::max(0.0f, std::min(1.0f, startRatio));
            endRatio = std::max(0.0f, std::min(1.0f, endRatio));
            
            renderer->setColor(dangerzone.color);
            int startY = gaugey + static_cast<int>(gaugeheight * (1.0f - endRatio));
            int height = static_cast<int>(gaugeheight * (endRatio - startRatio));
            renderer->drawRectangle(gaugex, startY, gaugewidth, height, true);
        }
    } else {
        // Draw circular/semicircular zones
        if (safezone.enabled) {
            float startAngle = valueToAngle(safezone.startValue);
            float endAngle = valueToAngle(safezone.endValue);
            renderer->setColor(safezone.color);
            renderer->drawArc(centerx, centery, radius * 0.9f, startAngle, endAngle, true, 32);
        }
        
        if (warningzone.enabled) {
            float startAngle = valueToAngle(warningzone.startValue);
            float endAngle = valueToAngle(warningzone.endValue);
            renderer->setColor(warningzone.color);
            renderer->drawArc(centerx, centery, radius * 0.9f, startAngle, endAngle, true, 32);
        }
        
        if (dangerzone.enabled) {
            float startAngle = valueToAngle(dangerzone.startValue);
            float endAngle = valueToAngle(dangerzone.endValue);
            renderer->setColor(dangerzone.color);
            renderer->drawArc(centerx, centery, radius * 0.9f, startAngle, endAngle, true, 32);
        }
    }
}

void gipGauge::drawTicks() {
    if (gaugetype == GAUGE_LINEAR_HORIZONTAL || gaugetype == GAUGE_LINEAR_VERTICAL) {
        // Linear gauge ticks can be implemented here if needed
        return;
    }
    
    renderer->setColor(tickcolor);
    
    // Draw major ticks
    std::vector<float> majorTickValues = generateTickValues(true);
    for (float value : majorTickValues) {
        float angle = valueToAngle(value);
        float cos_a = std::cos(angle * M_PI / 180.0f);
        float sin_a = std::sin(angle * M_PI / 180.0f);
        
        float innerRadius = radius * 0.85f;
        float outerRadius = radius * 0.95f;
        
        float x1 = centerx + innerRadius * cos_a;
        float y1 = centery + innerRadius * sin_a;
        float x2 = centerx + outerRadius * cos_a;
        float y2 = centery + outerRadius * sin_a;
        
        renderer->drawLine(x1, y1, x2, y2);
        
        // Draw tick labels
        if (showticklabels) {
            std::string label = formatValue(value);
            float labelRadius = radius * 0.75f;
            float labelX = centerx + labelRadius * cos_a;
            float labelY = centery + labelRadius * sin_a;
            
            renderer->drawText(label, labelX - 10, labelY - 5, gFont::FONT_SIZE_SMALL);
        }
    }
    
    // Draw minor ticks
    std::vector<float> minorTickValues = generateTickValues(false);
    for (float value : minorTickValues) {
        float angle = valueToAngle(value);
        float cos_a = std::cos(angle * M_PI / 180.0f);
        float sin_a = std::sin(angle * M_PI / 180.0f);
        
        float innerRadius = radius * 0.9f;
        float outerRadius = radius * 0.95f;
        
        float x1 = centerx + innerRadius * cos_a;
        float y1 = centery + innerRadius * sin_a;
        float x2 = centerx + outerRadius * cos_a;
        float y2 = centery + outerRadius * sin_a;
        
        renderer->drawLine(x1, y1, x2, y2);
    }
}

void gipGauge::drawNeedle() {
    if (gaugetype == GAUGE_LINEAR_HORIZONTAL || gaugetype == GAUGE_LINEAR_VERTICAL) {
        return;  // Linear gauges don't have needles
    }
    
    renderer->setColor(needlecolor);
    
    float angle = valueToAngle(currentvalue);
    float cos_a = std::cos(angle * M_PI / 180.0f);
    float sin_a = std::sin(angle * M_PI / 180.0f);
    
    float needleEndRadius = radius * needlelength;
    float needleEndX = centerx + needleEndRadius * cos_a;
    float needleEndY = centery + needleEndRadius * sin_a;
    
    // Draw needle line
    renderer->drawLine(centerx, centery, needleEndX, needleEndY);
    
    // Draw center dot
    renderer->drawCircle(centerx, centery, 3, true);
}

void gipGauge::drawValueText() {
    renderer->setColor(valuetextcolor);
    
    std::string valueText = formatValue(currentvalue);
    if (!unit.empty()) {
        valueText += " " + unit;
    }
    
    float textX, textY;
    if (gaugetype == GAUGE_SEMICIRCLE) {
        textX = centerx - 20;
        textY = centery - 20;
    } else if (gaugetype == GAUGE_LINEAR_HORIZONTAL) {
        textX = gaugex + gaugewidth * 0.5f - 20;
        textY = gaugey + gaugeheight * 0.5f;
    } else if (gaugetype == GAUGE_LINEAR_VERTICAL) {
        textX = gaugex + gaugewidth * 0.5f - 20;
        textY = gaugey + gaugeheight * 0.5f;
    } else {  // GAUGE_CIRCULAR
        textX = centerx - 20;
        textY = centery + 10;
    }
    
    renderer->drawText(valueText, textX, textY, gFont::FONT_SIZE_MEDIUM);
}

void gipGauge::drawTitle() {
    renderer->setColor(titlecolor);
    
    float titleX, titleY;
    if (gaugetype == GAUGE_SEMICIRCLE) {
        titleX = centerx - title.length() * 3;
        titleY = gaugey + 15;
    } else if (gaugetype == GAUGE_LINEAR_HORIZONTAL) {
        titleX = gaugex + 5;
        titleY = gaugey - 5;
    } else if (gaugetype == GAUGE_LINEAR_VERTICAL) {
        titleX = gaugex + 5;
        titleY = gaugey - 5;
    } else {  // GAUGE_CIRCULAR
        titleX = centerx - title.length() * 3;
        titleY = gaugey + 15;
    }
    
    renderer->drawText(title, titleX, titleY, gFont::FONT_SIZE_SMALL);
}

float gipGauge::valueToAngle(float value) {
    float valueRatio = (value - minvalue) / (maxvalue - minvalue);
    valueRatio = std::max(0.0f, std::min(1.0f, valueRatio));
    
    if (gaugetype == GAUGE_SEMICIRCLE) {
        return 180.0f + valueRatio * 180.0f;  // 180° to 360° (bottom semicircle)
    } else {
        return 225.0f + valueRatio * 270.0f;  // 225° to 495° (3/4 circle, starting from bottom-left)
    }
}

float gipGauge::angleToValue(float angle) {
    float normalizedAngle;
    if (gaugetype == GAUGE_SEMICIRCLE) {
        normalizedAngle = (angle - 180.0f) / 180.0f;
    } else {
        normalizedAngle = (angle - 225.0f) / 270.0f;
    }
    
    normalizedAngle = std::max(0.0f, std::min(1.0f, normalizedAngle));
    return minvalue + normalizedAngle * (maxvalue - minvalue);
}

gColor gipGauge::getZoneColorForValue(float value) {
    if (dangerzone.enabled && value >= dangerzone.startValue && value <= dangerzone.endValue) {
        return dangerzonecolor;
    }
    if (warningzone.enabled && value >= warningzone.startValue && value <= warningzone.endValue) {
        return warningzonecolor;
    }
    if (safezone.enabled && value >= safezone.startValue && value <= safezone.endValue) {
        return safezonecolor;
    }
    return gColor(0, 0, 0, 0);  // Transparent/no zone color
}

std::vector<float> gipGauge::generateTickValues(bool majorTicks) {
    std::vector<float> values;
    
    if (majorTicks) {
        for (int i = 0; i <= majortickcount; i++) {
            float value = minvalue + (maxvalue - minvalue) * i / majortickcount;
            values.push_back(value);
        }
    } else {
        float majorInterval = (maxvalue - minvalue) / majortickcount;
        float minorInterval = majorInterval / (minortickcount + 1);
        
        for (int i = 0; i < majortickcount; i++) {
            float majorValue = minvalue + majorInterval * i;
            for (int j = 1; j <= minortickcount; j++) {
                float minorValue = majorValue + minorInterval * j;
                if (minorValue < maxvalue) {
                    values.push_back(minorValue);
                }
            }
        }
    }
    
    return values;
}

std::string gipGauge::formatValue(float value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << value;
    return ss.str();
}

// Value management
void gipGauge::setValue(float value) {
    targetvalue = std::max(minvalue, std::min(maxvalue, value));
}

float gipGauge::getValue() const {
    return currentvalue;
}

void gipGauge::setMinValue(float minValue) {
    minvalue = minValue;
    if (targetvalue < minvalue) targetvalue = minvalue;
    if (currentvalue < minvalue) currentvalue = minvalue;
}

void gipGauge::setMaxValue(float maxValue) {
    maxvalue = maxValue;
    if (targetvalue > maxvalue) targetvalue = maxvalue;
    if (currentvalue > maxvalue) currentvalue = maxvalue;
}

float gipGauge::getMinValue() const {
    return minvalue;
}

float gipGauge::getMaxValue() const {
    return maxvalue;
}

void gipGauge::setValueRange(float minValue, float maxValue) {
    minvalue = minValue;
    maxvalue = maxValue;
    targetvalue = std::max(minvalue, std::min(maxvalue, targetvalue));
    currentvalue = std::max(minvalue, std::min(maxvalue, currentvalue));
}

// Visual customization
void gipGauge::setGaugeType(GaugeType type) {
    gaugetype = type;
}

gipGauge::GaugeType gipGauge::getGaugeType() const {
    return gaugetype;
}

void gipGauge::setGaugeStyle(GaugeStyle style) {
    gaugestyle = style;
}

gipGauge::GaugeStyle gipGauge::getGaugeStyle() const {
    return gaugestyle;
}

void gipGauge::setTitle(const std::string& titleText) {
    title = titleText;
}

std::string gipGauge::getTitle() const {
    return title;
}

void gipGauge::setUnit(const std::string& unitText) {
    unit = unitText;
}

std::string gipGauge::getUnit() const {
    return unit;
}

// Color setters
void gipGauge::setBackgroundColor(const gColor& color) {
    backgroundcolor = color;
}

void gipGauge::setNeedleColor(const gColor& color) {
    needlecolor = color;
}

void gipGauge::setTickColor(const gColor& color) {
    tickcolor = color;
}

void gipGauge::setValueTextColor(const gColor& color) {
    valuetextcolor = color;
}

void gipGauge::setTitleColor(const gColor& color) {
    titlecolor = color;
}

void gipGauge::setDangerZoneColor(const gColor& color) {
    dangerzonecolor = color;
    dangerzone.color = color;
}

void gipGauge::setWarningZoneColor(const gColor& color) {
    warningzonecolor = color;
    warningzone.color = color;
}

void gipGauge::setSafeZoneColor(const gColor& color) {
    safezonecolor = color;
    safezone.color = color;
}

// Color getters
gColor gipGauge::getBackgroundColor() const {
    return backgroundcolor;
}

gColor gipGauge::getNeedleColor() const {
    return needlecolor;
}

gColor gipGauge::getTickColor() const {
    return tickcolor;
}

gColor gipGauge::getValueTextColor() const {
    return valuetextcolor;
}

gColor gipGauge::getTitleColor() const {
    return titlecolor;
}

// Zone management
void gipGauge::setDangerZone(float startValue, float endValue) {
    dangerzone.startValue = startValue;
    dangerzone.endValue = endValue;
    dangerzone.enabled = true;
}

void gipGauge::setWarningZone(float startValue, float endValue) {
    warningzone.startValue = startValue;
    warningzone.endValue = endValue;
    warningzone.enabled = true;
}

void gipGauge::setSafeZone(float startValue, float endValue) {
    safezone.startValue = startValue;
    safezone.endValue = endValue;
    safezone.enabled = true;
}

void gipGauge::clearZones() {
    dangerzone.enabled = false;
    warningzone.enabled = false;
    safezone.enabled = false;
}

// Animation
void gipGauge::setAnimationEnabled(bool enabled) {
    animationenabled = enabled;
}

bool gipGauge::isAnimationEnabled() const {
    return animationenabled;
}

void gipGauge::setAnimationSpeed(float speed) {
    animationspeed = speed;
}

float gipGauge::getAnimationSpeed() const {
    return animationspeed;
}

// Tick management
void gipGauge::setMajorTickCount(int count) {
    majortickcount = std::max(2, count);
}

void gipGauge::setMinorTickCount(int count) {
    minortickcount = std::max(0, count);
}

int gipGauge::getMajorTickCount() const {
    return majortickcount;
}

int gipGauge::getMinorTickCount() const {
    return minortickcount;
}

void gipGauge::setShowTickLabels(bool show) {
    showticklabels = show;
}

bool gipGauge::getShowTickLabels() const {
    return showticklabels;
}

// Display options
void gipGauge::setShowValue(bool show) {
    showvalue = show;
}

void gipGauge::setShowTitle(bool show) {
    showtitle = show;
}

void gipGauge::setShowNeedle(bool show) {
    showneedle = show;
}

bool gipGauge::getShowValue() const {
    return showvalue;
}

bool gipGauge::getShowTitle() const {
    return showtitle;
}

bool gipGauge::getShowNeedle() const {
    return showneedle;
}

// Needle properties
void gipGauge::setNeedleLength(float length) {
    needlelength = std::max(0.1f, std::min(1.0f, length));
}

void gipGauge::setNeedleWidth(float width) {
    needlewidth = std::max(1.0f, width);
}

float gipGauge::getNeedleLength() const {
    return needlelength;
}

float gipGauge::getNeedleWidth() const {
    return needlewidth;
}