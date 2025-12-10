# gipGauge Plugin Example

This example demonstrates how to use the gipGauge plugin in your GlistEngine project.

## Basic Usage

```cpp
#include "gipGauge.h"

class YourCanvas : public gBaseCanvas {
private:
    gipGauge speedGauge;
    gipGauge temperatureGauge;
    gipGauge fuelGauge;
    
public:
    void setup() {
        // Setup speed gauge (circular)
        speedGauge.setTitle("Speed");
        speedGauge.setUnit("km/h");
        speedGauge.setValueRange(0, 200);
        speedGauge.setGaugeType(gipGauge::GAUGE_CIRCULAR);
        speedGauge.setNeedleColor(gColor::red);
        speedGauge.setDangerZone(150, 200);
        speedGauge.setWarningZone(100, 150);
        speedGauge.setSafeZone(0, 100);
        
        // Setup temperature gauge (semicircle)
        temperatureGauge.setTitle("Engine Temp");
        temperatureGauge.setUnit("°C");
        temperatureGauge.setValueRange(-20, 120);
        temperatureGauge.setGaugeType(gipGauge::GAUGE_SEMICIRCLE);
        temperatureGauge.setNeedleColor(gColor::blue);
        temperatureGauge.setDangerZone(90, 120);
        temperatureGauge.setWarningZone(70, 90);
        temperatureGauge.setSafeZone(-20, 70);
        
        // Setup fuel gauge (linear horizontal)
        fuelGauge.setTitle("Fuel Level");
        fuelGauge.setUnit("L");
        fuelGauge.setValueRange(0, 60);
        fuelGauge.setGaugeType(gipGauge::GAUGE_LINEAR_HORIZONTAL);
        fuelGauge.setDangerZone(0, 10);
        fuelGauge.setWarningZone(10, 20);
        fuelGauge.setSafeZone(20, 60);
    }
    
    void update() {
        // Simulate changing values
        static float time = 0;
        time += 0.016f; // Assuming 60 FPS
        
        // Update gauge values with simulation
        float speed = 80 + 50 * sin(time * 0.5f);
        float temp = 60 + 20 * sin(time * 0.3f);
        float fuel = 30 + 15 * cos(time * 0.2f);
        
        speedGauge.setValue(speed);
        temperatureGauge.setValue(temp);
        fuelGauge.setValue(fuel);
    }
    
    void draw() {
        // Draw gauges at different positions
        speedGauge.draw(50, 50, 200, 200);      // Circular gauge
        temperatureGauge.draw(300, 50, 200, 200); // Semicircle gauge
        fuelGauge.draw(50, 300, 450, 50);        // Linear horizontal gauge
    }
};
```

## Advanced Customization

```cpp
void setupAdvancedGauge() {
    gipGauge advancedGauge;
    
    // Customize appearance
    advancedGauge.setTitle("Advanced Gauge");
    advancedGauge.setUnit("units");
    advancedGauge.setValueRange(0, 100);
    
    // Colors
    advancedGauge.setBackgroundColor(gColor(0.1f, 0.1f, 0.2f, 1.0f));
    advancedGauge.setNeedleColor(gColor(1.0f, 0.8f, 0.0f, 1.0f)); // Gold
    advancedGauge.setTickColor(gColor(0.8f, 0.8f, 0.9f, 1.0f));
    advancedGauge.setValueTextColor(gColor::white);
    advancedGauge.setTitleColor(gColor::lightGray);
    
    // Tick configuration
    advancedGauge.setMajorTickCount(10);
    advancedGauge.setMinorTickCount(4);
    advancedGauge.setShowTickLabels(true);
    
    // Animation
    advancedGauge.setAnimationEnabled(true);
    advancedGauge.setAnimationSpeed(3.0f);
    
    // Needle properties
    advancedGauge.setNeedleLength(0.9f);
    advancedGauge.setNeedleWidth(2.0f);
    
    // Display options
    advancedGauge.setShowValue(true);
    advancedGauge.setShowTitle(true);
    advancedGauge.setShowNeedle(true);
}
```

## Gauge Types

1. **GAUGE_CIRCULAR**: Full 270° circular gauge with needle
2. **GAUGE_SEMICIRCLE**: 180° semicircular gauge 
3. **GAUGE_LINEAR_HORIZONTAL**: Horizontal bar gauge
4. **GAUGE_LINEAR_VERTICAL**: Vertical bar gauge

## Features

- **Multiple gauge types**: Circular, semicircle, and linear gauges
- **Smooth animations**: Configurable animation speed for value transitions
- **Color zones**: Define danger, warning, and safe zones with custom colors
- **Customizable appearance**: Colors, needle properties, tick marks
- **Value formatting**: Automatic value formatting with units
- **Tick marks**: Configurable major and minor tick marks with labels
- **Responsive design**: Adapts to any size and position

## Zone Configuration

```cpp
// Set up color zones for visual feedback
gauge.setSafeZone(0, 60);      // Green zone
gauge.setWarningZone(60, 80);   // Yellow zone  
gauge.setDangerZone(80, 100);   // Red zone

// Customize zone colors
gauge.setSafeZoneColor(gColor(0.2f, 1.0f, 0.2f, 0.3f));
gauge.setWarningZoneColor(gColor(1.0f, 0.8f, 0.0f, 0.3f));
gauge.setDangerZoneColor(gColor(1.0f, 0.2f, 0.2f, 0.3f));
```