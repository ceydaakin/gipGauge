# gipGauge - Advanced Gauge Plugin for GlistEngine

A comprehensive gauge plugin for GlistEngine that provides multiple types of gauges with rich customization options, animations, and zone-based visual feedback.

## Features

- **Multiple Gauge Types**: Circular, semicircle, linear horizontal, and linear vertical gauges
- **Smooth Animations**: Configurable animation speed for smooth value transitions
- **Color Zones**: Define danger, warning, and safe zones with custom colors
- **Rich Customization**: Colors, fonts, needle properties, tick marks, and more
- **Value Formatting**: Automatic formatting with customizable units
- **Responsive Design**: Adapts to any size and position
- **Performance Optimized**: Efficient rendering with minimal overhead

## Installation

Clone this repository under your GlistEngine plugins directory:
```bash
cd ~/dev/glist/glistplugins
git clone https://github.com/ceydaakin/gipGauge.git
```

For Windows developers, add the following directory to your GlistApp project's PATH:
```
${workspace_loc}\..\..\..\..\glistplugins\gipGauge\libs\bin
```
(Project->Properties->C/C++ Build->Environment->PATH)

## Quick Start

```cpp
#include "gipGauge.h"

class YourCanvas : public gBaseCanvas {
private:
    gipGauge speedometer;
    
public:
    void setup() {
        speedometer.setTitle("Speed");
        speedometer.setUnit("km/h");
        speedometer.setValueRange(0, 200);
        speedometer.setGaugeType(gipGauge::GAUGE_CIRCULAR);
        speedometer.setDangerZone(150, 200);
        speedometer.setWarningZone(100, 150);
        speedometer.setSafeZone(0, 100);
    }
    
    void update() {
        // Update gauge value (animates smoothly)
        speedometer.setValue(getCurrentSpeed());
    }
    
    void draw() {
        speedometer.draw(100, 100, 300, 300);
    }
};
```

## Gauge Types

### Circular Gauge (`GAUGE_CIRCULAR`)
- Full 270° circular display with rotating needle
- Classic speedometer/tachometer style
- Configurable tick marks and labels

### Semicircle Gauge (`GAUGE_SEMICIRCLE`)  
- 180° semicircular display
- Space-efficient design
- Perfect for dashboards

### Linear Horizontal (`GAUGE_LINEAR_HORIZONTAL`)
- Horizontal progress bar style
- Great for levels, progress indicators
- Zone colors displayed as background segments

### Linear Vertical (`GAUGE_LINEAR_VERTICAL`)
- Vertical progress bar style  
- Ideal for temperature, fuel level displays
- Bottom-to-top value representation

## Advanced Features

### Color Zones
Define visual zones for instant value assessment:
```cpp
gauge.setSafeZone(0, 60);      // Green zone (safe operation)
gauge.setWarningZone(60, 80);   // Yellow zone (caution)  
gauge.setDangerZone(80, 100);   // Red zone (danger)
```

### Animation System
Smooth value transitions with configurable speed:
```cpp
gauge.setAnimationEnabled(true);
gauge.setAnimationSpeed(5.0f); // Higher = faster transitions
```

### Customization Options
```cpp
// Colors
gauge.setBackgroundColor(gColor::darkGray);
gauge.setNeedleColor(gColor::red);
gauge.setTickColor(gColor::white);

// Tick marks  
gauge.setMajorTickCount(10);
gauge.setMinorTickCount(5);
gauge.setShowTickLabels(true);

// Needle properties
gauge.setNeedleLength(0.8f); // 80% of radius
gauge.setNeedleWidth(3.0f);  // 3 pixels wide

// Display options
gauge.setShowValue(true);
gauge.setShowTitle(true);
```

## API Reference

### Core Methods
- `setValue(float)` - Set gauge value (animates to target)
- `setValueRange(float min, float max)` - Set value range  
- `setGaugeType(GaugeType)` - Change gauge type
- `draw(int x, int y, int w, int h)` - Render gauge

### Zone Management
- `setDangerZone(float start, float end)` - Define danger zone
- `setWarningZone(float start, float end)` - Define warning zone  
- `setSafeZone(float start, float end)` - Define safe zone
- `clearZones()` - Remove all zones

### Visual Customization
- `setTitle(string)` / `setUnit(string)` - Labels
- `setBackgroundColor(gColor)` - Background color
- `setNeedleColor(gColor)` - Needle/bar color
- `setTickColor(gColor)` - Tick mark color

### Animation Control
- `setAnimationEnabled(bool)` - Enable/disable animation
- `setAnimationSpeed(float)` - Animation speed multiplier

## Examples

See the `examples/` directory for comprehensive usage examples including:
- Basic gauge setup
- Multi-gauge dashboard  
- Advanced customization
- Real-time data integration

## Requirements

- GlistEngine framework
- C++11 or later
- OpenGL-compatible graphics

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests, report bugs, or suggest new features.
