---
title: Frequently asked questions
weight: 50
chapter: true
---

## Frequently Asked Questions

**Who is Kiso for?**

We have a page on that. Please have a look at [Kiso's Scope]({{< ref "../Project overview/scope.md" >}}).

**Why is the Bosch Cross Domain Development Kit (XDK aka. XDK110) not a Kiso reference HW?**

Actually, the XDK is a Kiso reference HW. Kiso and the XDK share the same SW basis, i.e., the Bosch Connected Devices Development Kit (**CDDK**). But, XDK and Kiso have different targeted users in mind. The XDK is a complete IoT development bundle consisting of HW, SW, IDE, community and support. Whereas Kiso is a pure SW SDK that supports multiple different reference HW.

The repo, code structure and build system are basically incompatible, e.g., Kiso is only having a single repo (see [Kiso's quality goals]({{< ref "rules_for_development.md" >}}#our-quality-goals)) in contrast to the multi-repo of XDK. The latter is necessary to compile and package the proprietary licence code SW modules as libraries. Also, the XDK examples are solely tailored for the XDK110 HW and it's extension boards.

Anyway, we're continuously working on extending the Kiso reference HW basis. Thus, we expect to also include the XDK with a EPLv2 BSP, drivers and examples here in Kiso, soonTM.

------
We're still collecting more frequently asked questions.
Please feel free to contact us via Mattermost and update us about your questions. (see  [Kiso Channel]({{< ref "communication.md" >}})).

**NOTE:**   An eclipse foundation Mattermost account is needed.