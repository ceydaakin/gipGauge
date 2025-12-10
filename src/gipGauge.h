/*
 * gipGauge.h
 *
 *  Created on: Mar 11, 2023
 *      Author: Noyan Culum
 *      Created by: kayra
 */

#ifndef SRC_GIPGAUGE_H_
#define SRC_GIPGAUGE_H_

#include "gBasePlugin.h"
#include "gColor.h"
#include <vector>
#include <string>

class gipGauge : public gBasePlugin {
public:
    // Gauge types
    enum GaugeType {
        GAUGE_CIRCULAR = 0,
        GAUGE_SEMICIRCLE,
        GAUGE_LINEAR_HORIZONTAL,
        GAUGE_LINEAR_VERTICAL
    };

    // Gauge style
    enum GaugeStyle {
        STYLE_MODERN = 0,
        STYLE_CLASSIC,
        STYLE_DIGITAL,
        STYLE_MINIMALIST
    };

    gipGauge();
    virtual ~gipGauge();

    // Core drawing functions
    void draw(int x, int y, int w, int h);
    void update();

    // Value management
    void setValue(float value);
    float getValue() const;
    void setMinValue(float minValue);
    void setMaxValue(float maxValue);
    float getMinValue() const;
    float getMaxValue() const;
    void setValueRange(float minValue, float maxValue);

    // Visual customization
    void setGaugeType(GaugeType type);
    GaugeType getGaugeType() const;
    void setGaugeStyle(GaugeStyle style);
    GaugeStyle getGaugeStyle() const;
    
    void setTitle(const std::string& title);
    std::string getTitle() const;
    void setUnit(const std::string& unit);
    std::string getUnit() const;

    // Colors
    void setBackgroundColor(const gColor& color);
    void setNeedleColor(const gColor& color);
    void setTickColor(const gColor& color);
    void setValueTextColor(const gColor& color);
    void setTitleColor(const gColor& color);
    void setDangerZoneColor(const gColor& color);
    void setWarningZoneColor(const gColor& color);
    void setSafeZoneColor(const gColor& color);

    gColor getBackgroundColor() const;
    gColor getNeedleColor() const;
    gColor getTickColor() const;
    gColor getValueTextColor() const;
    gColor getTitleColor() const;

    // Zones for color coding
    void setDangerZone(float startValue, float endValue);
    void setWarningZone(float startValue, float endValue);
    void setSafeZone(float startValue, float endValue);
    void clearZones();

    // Animation
    void setAnimationEnabled(bool enabled);
    bool isAnimationEnabled() const;
    void setAnimationSpeed(float speed);
    float getAnimationSpeed() const;

    // Tick marks
    void setMajorTickCount(int count);
    void setMinorTickCount(int count);
    int getMajorTickCount() const;
    int getMinorTickCount() const;
    void setShowTickLabels(bool show);
    bool getShowTickLabels() const;

    // Display options
    void setShowValue(bool show);
    void setShowTitle(bool show);
    void setShowNeedle(bool show);
    bool getShowValue() const;
    bool getShowTitle() const;
    bool getShowNeedle() const;

    // Size and positioning
    void setNeedleLength(float length);
    void setNeedleWidth(float width);
    float getNeedleLength() const;
    float getNeedleWidth() const;

private:
    // Core properties
    float currentvalue;
    float targetvalue;
    float minvalue;
    float maxvalue;
    
    // Visual properties
    GaugeType gaugetype;
    GaugeStyle gaugestyle;
    std::string title;
    std::string unit;

    // Colors
    gColor backgroundcolor;
    gColor needlecolor;
    gColor tickcolor;
    gColor valuetextcolor;
    gColor titlecolor;
    gColor dangerzonecolor;
    gColor warningzonecolor;
    gColor safezonecolor;

    // Zones
    struct ValueZone {
        float startValue;
        float endValue;
        gColor color;
        bool enabled;
    };
    ValueZone dangerzone;
    ValueZone warningzone;
    ValueZone safezone;

    // Animation
    bool animationenabled;
    float animationspeed;
    
    // Ticks
    int majortickcount;
    int minortickcount;
    bool showticklabels;

    // Display flags
    bool showvalue;
    bool showtitle;
    bool showneedle;

    // Needle properties
    float needlelength;
    float needlewidth;

    // Drawing dimensions
    int gaugex, gaugey, gaugewidth, gaugeheight;
    float centerx, centery, radius;

    // Helper methods
    void initializeDefaults();
    void updateAnimation();
    float lerp(float start, float end, float t);
    
    // Drawing helpers
    void drawCircularGauge();
    void drawSemicircleGauge();
    void drawLinearGaugeHorizontal();
    void drawLinearGaugeVertical();
    
    void drawBackground();
    void drawTicks();
    void drawNeedle();
    void drawValueText();
    void drawTitle();
    void drawZones();
    
    // Utility functions
    float valueToAngle(float value);
    float angleToValue(float angle);
    gColor getZoneColorForValue(float value);
    std::vector<float> generateTickValues(bool majorTicks);
    std::string formatValue(float value);
};

#endif /* SRC_GIPGAUGE_H_ */