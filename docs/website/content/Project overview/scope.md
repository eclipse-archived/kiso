---
title: "Scope"
weight: 10
---

Who and how can Kiso help? Let's look at some user personas to answer this.

## Meet Peter, the embedded device developer ##

Peter is an experienced SW developer at a device manufacturer. The company's business model is selling IoT devices to
business customers as well as directly to end users. These IoT devices are based on different MCUs with quite
constrained system resources. They are battery driven and have at least one wireless connectivity. The typical use case
scenario of these devices is to collect, process and then send data out via a radio interface. The data is generated via
sensors or other HW that it connected via serial interface. Peter is developing the SW for these devices - sounds like
fun.

A full scale device development is most of the time preceded by a prototype. Martina, a friendly cloud developer, is
frequently demoing her companies services with her self-made prototypes. These demo prototypes are using a lot of system
resources without any power optimization or without considering a commercial use of SW licenses. So, these prototypes
are basically of no re-use for Peter.

Thus, Peter typically took some RTOS and integrated HW manufacturer drivers when available. If no drivers had been
available he wrote them by himself. Over the last couple of years he build up a collection of SW modules and drivers.
All of this SW modules and drivers are specifically designed for a particular use case and device. For a new project he
copies over parts of his existing SW set. But, most of these SW parts need modifications in order to fit the new device
and to become compatible to each other again in a new use case setup.

Now, Peter read a newsletter about a new SW development kit on the block. Yeah! He found out that Kiso has a harmonized
and compatible set of SW modules and drivers he can choose from. Also, Kiso is very well documented to easily and
quickly start a new SW development project (even though Peter would never admit that he reads the documentation before
the source code). The well tested SW gives him the comfort of Bosch quality and let him focus on bug hunting on his
self-written code. He's now replacing parts of his existing SW set one by one with Kiso components.

He now has much less - if not none - effort to build up a SW basis that he can start developing the IoT device
application on. It's very convenient for him that he can just clone from an open source repository without worrying
about licenses. All these benefits help Peter to minimize the technical debts, enable adequate development costs
together with a shorter time to market (TTM). Thus, he's so very happy right now and exited committing his improved SW
modules and additional drivers back to the Kiso project. He becomes an active Kiso contributor.

## Meet Martina, the cloud developer ##

This story is in review and will be published soon<sup>TM</sup>.

## Meet Aditya, the MEMS engineer ##

This story is in review and will be published soon<sup>TM</sup>.
