---
title: "Principles for development"
description: "Different rules that needs to be taken care of"
weight: 6
draft: false
toc: true
menu:
  main:
    parent: 4. Contribution guide
    identifier: what_are_the_rules_i_need_to_take_care_of_when_i_contribute
    weight: 6
---

# What are the rules I need to take care of when I contribute?

To ensure quality, all contributors need to follow certain set of rules while working on Kiso.

## Our coding guidelines

Like most open source projects, Kiso has its own coding guidelines. This topic is subjective and nobody can find a coding style that suites for every developer, so let’s agree on disagree and please following Kiso coding convention when you write Kiso code. (link tbd)
## Our Definition of Done

For each Kiso source code contribution, Kiso's DoD needs to be fulfilled. (link tbd)
## Individual module principles

Except general requirements descripted above, some modules have their own development guides which needs to be followed to ensure quality and consistency.
## Our quality goals

It is important for you to have them in mind. Design discussions (for new modules or the extension of old modules ) will be driven by the quality goals.

| Rank | Quality goals       | Description                                                                                  |
| :--- | :------------------ | :------------------------------------------------------------------------------------------- |
| 1    | **Simplicity**      | Easy to understand how Kiso is built and how to contribute to it                             |
| 2    | **Robustness**      | No crash in case of stress or unexpected conditions, full transparency in case of errors     |
| 3    | **Usability**       | Easy to integrate and easy to use in a project, low entry barrier                            |
| 4    | **Maintainability** | Ability to increase the feature-set and ability to fix bugs in a short amount of time        |
| 5    | **Modularity**      | Ability to encapsulate features in modules and to select which module to use in the project  |
| 6    | **Extensibility**   | The modules are build in a way that adding new features is straightforward                   |
| 7    | **Portability**     | Kiso support applications to be portable to other hardware                                   |
| 8    | **Performance**     | Modules are designed to run in a very resource constrained environment (energy, memory, CPU) |
| 9    | **Configurability** | Each module can be configured for specific application purposes                              |

