---
title: "DoD"
weight: 8
---

# Definition of Done


## When is my piece of software "done"?

The following "Definition of Done" (DoD) guides the development teams to judge on the development of a piece of software (i.e., user story, task, bug, etc.) being finished or "Done". Note that the DoD is a mindset and guideline rather than a checklist. Our goal and our own responsibility is to develop quality software. This DoD helps us to achieve this goal.

## Definition of Done (DoD)

1. The design (both structural and behavioural) has been developed and approved by at least 2 of the KISO commiters.
2. All expected functions are implemented
3. Code review has been conducted and accepted
4. Relevant documents such as design documentation, readme, user guide, etc. are up-to-date
5. The dependencies between libraries are clean, up-to-date
6. Adequate feature tests (integration/system tests) are defined, implemented and passed.
7. A proper continuous integration setup is available and running, including build, testing and reporting
8. The continuous integration confirms the health of the software:
  - a. Build is successful
  - b. No compiler warning
  - c. No static analysis issues
  - d. Successful unit tests
  - e. Sufficient unit test coverage (90% line, 80% conditional)
  - f. Successful integration tests
  - g. Comprehensive and tidy reports
  - h. OSS scan report

9. Known defects, if there are any, are reported in bug tracking system

## Exceptions and Relaxations

In certain situations, some aspects of the definition of done may be a subject of compromise. Such exceptions and relaxations need to be agreed in the review meeting.

## Non-Embedded Code

This definition of done handles embedded product code in the first place. For non-embedded code such as smartphone apps, server components, tools and scripts the definition of done may/should be tailored to meet the requirements of the particular case.
