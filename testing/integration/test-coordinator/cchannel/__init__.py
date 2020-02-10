"""
Channel Package Interface
********************************
 
:module: cchannel
 
:synopsis: Create a channel based on a string
 
.. currentmodule:: cchannel
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0

.. warning:: Still under test
"""
# Import based on the caller
import sys, os
sys.path.append(os.path.dirname(__file__))

# Channels
import cc_example
import cc_uart
import cc_udp
import cc_usb

# Dictionary to reach the classes
cchannel_dict = {
    "cc_example":cc_example.CCExample,
    "cc_uart":cc_uart.CCUart,
    "cc_udp":cc_udp.CCUdp,
    "cc_usb":cc_usb.CCUsb
}
