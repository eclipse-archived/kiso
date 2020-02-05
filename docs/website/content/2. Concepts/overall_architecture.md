---
title: "Overall architecture"
weight: 1
---

## The challenge
Kiso was built to answer the challenges that comes with the development of the IoT market. It was built with a strong focus on the constraints that IoT devices have. Its key features are:

- Battery management
- Small memory footprint
- Broad sensor support
- Broad MCUs support
- Broad connectivity support
- Firmware update mechanisms and device management

The key challenges in defining the architecture were:

- How can we design enough abstraction from the hardware to have generic applications running on specific hardware?
- How can we design Kiso in a way that supports application development but does enforce strict behaviour patterns (How to stream data, etc.)
- What should be Kiso code, what should be application code?
- Which components should exist within Kiso, what do they contain?

We also need to consider [our quality goals]({{< ref "rules_for_development.md" >}}#our-quality-goals)

## The abstracted overview
![overview_abstract_kiso-error](/images/overview_abstract_kiso.png)

## The components overview

![overview_component_kiso-error](/images/overview_component_kiso.png)

![legend_kiso-error](/images/legend.png)

## The component's expected design
### Base pattern (for all)
![component_base_pattern-kiso-error](/images/component_base_pattern.png)
**note**:
Each component, besides their outside dependencies, are free to choose its architecture based on its owns needs.

### Base components (Essentials, Utils)
![base_layer_component-kiso-error](/images/base_layer_component.png)

### Low layer components (Drivers, Cellular, ...)
![low_layer_component-kiso-error](/images/low_layer_component.png)

### High layer components (ServalPAL)
![high_layer_component-kiso-error](/images/high_layer_component.png)


