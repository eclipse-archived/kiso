---
title: "Principles for development"
weight: 4
---

# What are the rules I need to take care of when I contribute?

To ensure quality, all contributors need to follow certain set of rules while working on Kiso.

## Our coding guidelines

Like most open source projects, Kiso has its own coding guidelines. This topic is subjective and nobody can find a coding style that suites for every developer, so let’s agree on disagree, and please follow Kiso coding conventions when you write Kiso code.[C Code Guidelines] (../c_coding_guidelines)
## Our Definition of Done 

For each Kiso source code contribution, Kiso's DoD needs to be fulfilled. [DoD](../dod)
## Individual module principles

Except general requirements descripted above, some modules have their own development guides which needs to be followed to ensure quality and consistency.
## Our quality goals

It is important for you to have them in mind. Design discussions (for new modules or the extension of old modules) will be driven by the quality goals.

| Rank | Quality goals         | Sub-attribute       | Description                                                                                  |
| :--- | :-------------------- | :------------------ | :------------------------------------------------------------------------------------------- |
| 1    | **Understandability** | **Simplicity**      | Easy to understand how Kiso is built and how to contribute to it                             |
| 2    | **Usability**         | **Configurability** | Each module can be configured for specific application purposes                              |
|      |                       | **Portability**     | Kiso support applications to be portable to other hardware                                   |
| 3    | **Product quality**   | **Testability**     | Degree to which an artifact can be tested                                                    |
|      |                       | **Maintainability** | Ability to increase the feature-set and ability to fix bugs in a short amount of time        |
|      |                       | **Robustness**      | No crash in case of stress or unexpected conditions, full transparency in case of errors     |
| 4    | **Extensibility**     | **-**               | The modules are build in a way that adding new features is straightforward                   |
|      |                       | **Scalability**     | Ability to increase the number of work within a system                                       |
| 5    | **Performance**       |  **-**              | Modules are designed to run in a very resource constrained environment (energy, memory, CPU) |


