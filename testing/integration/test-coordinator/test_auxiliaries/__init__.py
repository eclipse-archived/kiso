"""
Auxiliary Package Interface
********************************
 
:module: test_auxiliaries
 
:synopsis: Create an auxiliary based on a string
 
.. currentmodule:: test_auxiliaries
 
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

# Auxiliaries
import example_test_auxiliary

# Dictionary to reach the classes
auxiliary_dict = {
    "example_test_auxiliary":example_test_auxiliary.ExampleAuxiliary
}
