"""
Message 
********
 
:module: message
 
:synopsis: Message that will be send though the different agents
 
.. currentmodule:: message
 
:Copyright: Copyright (c) 2010-2019 Robert Bosch GmbH
    This program and the accompanying materials are made available under the
    terms of the Eclipse Public License 2.0 which is available at
    http://www.eclipse.org/legal/epl-2.0.
    
    SPDX-License-Identifier: EPL-2.0
"""

import enum
import logging
import struct

message_counter = 0 # Will be used as token

@enum.unique
class MessageType(enum.IntEnum):
    """ List of messages allowed
    """
    COMMAND = 0
    REPORT = 1
    ACK = 2
    LOG = 3

@enum.unique
class MessageCommandType(enum.IntEnum):
    """ List of commands allowed
    """
    # Ping
    PING = 0
    # Setups
    TEST_SECTION_SETUP = 1
    TEST_SUITE_SETUP = 2
    TEST_CASE_SETUP = 3
    # Runs
    TEST_SECTION_RUN = 11
    TEST_SUITE_RUN = 12
    TEST_CASE_RUN = 13
    # Teardowns
    TEST_SECTION_TEARDOWN = 21
    TEST_SUITE_TEARDOWN = 22
    TEST_CASE_TEARDOWN = 23
    # Abort
    ABORT = 99

@enum.unique
class MessageReportType(enum.IntEnum):
    """ List of possible recieved messages
    """
    TEST_PASS = 0
    TEST_FAILED = 1

@enum.unique
class MessageAckType(enum.IntEnum):
    """ List of possible recieved messages
    """
    ACK = 0
    NACK = 1

@enum.unique
class TlvKnownTags(enum.IntEnum):
    """ List of known / supported tags
    """
    TEST_REPORT = 110
    FAILURE_REASON = 112

# Link types and sub-types
type_sub_type_dict = {
    MessageType.COMMAND: MessageCommandType,
    MessageType.REPORT: MessageReportType,
    MessageType.ACK: MessageAckType,
    MessageType.LOG: 0
}

class Message(object):
    """ A message

    The created message is a tlv style message with the following format:
    TYPE: msg_type | message_token | sub_type | errorCode | 

    :raise:
        * The message-type and sub-type are linked. a wrong combination can rais an error

    """
    def __init__(self, msg_type=0, sub_type=0, error_code=0,
                 test_section=0, test_suite=0, test_case=0, tlv_dict=None):
        """ Create a generic message

        :param msg_type: Message type
        :type msg_type: MessageType

        :param sub_type: Message sub-type
        :type sub_type: Message<MessageType>Type

        :param error_code: Error value
        :type error_code: integer

        :param test_section: Section value
        :type test_section: integer

        :param test_suite: Suite value
        :type test_suite: integer

        :param test_case: Test value
        :type test_case: integer

        :param tlv_dict: Dictionary containing tlvs elements in the form {'type':'value', ...}
        :type tlv_dict: dict
        """
        self._msg_type = msg_type
        global message_counter
        self._msg_token = (message_counter + 1) % 256
        message_counter += 1
        self._sub_type = sub_type
        self._error_code = error_code
        self._test_section = test_section
        self._test_suite = test_suite
        self._test_case = test_case
        self._tlv_dict = tlv_dict

    def __str__(self):
        """ String representation of a message object
        """
        m = 'msg_type:{}, message_token:{}, type:{}, error_code:{}, test_section ID:{}, test_suite ID:{}, test_case ID:{}'.format(
            self._msg_type, self._msg_token, self._sub_type, self._error_code, self._test_section, self._test_suite, self._test_case)
        if self._tlv_dict is not None:
            m += ', tlv_dict:{}'.format(self._tlv_dict)
        return m

    def serialize(self):
        """ Serialize message into raw packet

        Format: | msg_type (1b)     | msg_token (1b)  | sub_type (1b)  | error_code (1b)     |
                | test_section (1b) | test_suite (1b) | test_case (1b) | payload_length (1b) |
                | tlv_type (1b)     | tlv_size (1b)   | ...

        """
        raw_packet = b''
        raw_packet += struct.pack('B', ((int(self._msg_type) << 4) | (1 << 6)))
        raw_packet += struct.pack('B', self._msg_token)
        raw_packet += struct.pack('B', int(self._sub_type))
        raw_packet += struct.pack('B', self._error_code)
        raw_packet += struct.pack('B', self._test_section)
        raw_packet += struct.pack('B', self._test_suite)
        raw_packet += struct.pack('B', self._test_case)
        # Calculate and convert the dictionaries tlv elements into bytes
        if self._tlv_dict is not None:
            payload = b''
            for key, value in self._tlv_dict.items():
                # Check first if it the dict is conform
                parsed_key = b''
                if type(key) == TlvKnownTags:
                    parsed_key = struct.pack('B', int(key))
                else:
                    logging.warning("{} is not a supported format".format(key))
                parsed_value = b''
                if type(value) == str: # If string given
                    parsed_value = parsed_value.join([struct.pack( 'B', ord(val)) for val in value])
                elif type(value) == int:
                    parsed_value = struct.pack('H', value) # TODO check endianess later on
                elif type(value) == bytes:
                    parsed_value = value
                else:
                    logging.warning("{} is not a supported format".format(value))
                # Add the TLV element:
                payload += parsed_key
                payload += struct.pack('B', len(parsed_value))
                payload += parsed_value
            # Add the tlvs elements within the raw_packet
            raw_packet += struct.pack('B', len(payload))
            raw_packet += payload
        else:
            # Add the payload length to 0
            raw_packet += struct.pack('B', 0)
        return raw_packet

    def parse_packet(self, raw_packet):
        """ Populate from the raw data this message

        :param raw_packet: array of a received message
        :type raw_packet: bytes

        :return: itself
        """
        # Check if packet to parse is valid
        if type(raw_packet) != list and len(raw_packet) < 8:
            logging.error("Packet is not understandable")
        # Fill the message with the raw_packet
        self._msg_type = (MessageType)(int((raw_packet[0] & 0x30) >> 4))
        self._msg_token = int(raw_packet[1])
        # Because the sub-type depend on the type:
        self._sub_type = (type_sub_type_dict[self._msg_type])(int(raw_packet[2]))
        self._error_code = int(raw_packet[3])
        self._test_section = int(raw_packet[4])
        self._test_suite = int(raw_packet[5])
        self._test_case = int(raw_packet[6])
        payload_length = int(raw_packet[7])
        # Create payload based on known tlvs
        if payload_length != 0:
            self._tlv_dict = {}
            payload = raw_packet[8:]
            while len(payload) >=3:
                # Get size
                size = int(payload[1])
                # Create dict entry
                self._tlv_dict[(TlvKnownTags)(int(payload[0]))] = payload[2:(size+2)]
                # Jump the needed bytes
                payload = payload[(size+2):]
        # For saving time, we pass back the message
        return self

    def generate_ack_message(self, ack_type):
        """ Generate acknowledgement to send out

        :param ack_type: ack or nack
        :type ack_type: MessageAckType
        """
        # Return if wrong parameter given
        if type(ack_type) != MessageAckType:
            return None
        # Create ack message
        ack_message = Message(msg_type=MessageType.ACK, sub_type=ack_type, error_code=0, test_section=self._test_section, test_suite=self._test_suite, test_case=self._test_case)
        ack_message._msg_token = self._msg_token
        # Return the ack message
        return ack_message
    
    def check_if_ack_message_is_matching(self, ack_message):
        """ Check if the ack message was for this sent message
        """
        if ack_message._msg_type == MessageType.ACK and ack_message._msg_token == self._msg_token:
            return True
        else:
            logging.info("ack_message: {} \ndifferent of \nthis message: {}".format(str(ack_message), str(self)))
            return False
    
    def get_message_type(self):
        return self._msg_type

    def get_message_token(self):
        return self._msg_token

    def get_message_sub_type(self):
        return self._sub_type

    def get_message_tlv_dict(self):
        return self._tlv_dict

##################################### TEST SECTION ##########################################
import unittest

class MessageTest(unittest.TestCase):
    def test_message_creation(self):
        message_for_test = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3)
        print(message_for_test) # Not best unittest -> Used to compare with old code

    def test_message_consistency_with_tlv_dict(self):
        # Create the message
        tlv_dict_to_send = {TlvKnownTags.TEST_REPORT: 'OK', TlvKnownTags.FAILURE_REASON: b'\x12\x34\x56'}
        message_for_test = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3, tlv_dict=tlv_dict_to_send)
        # Check message content
        self.assertEqual(MessageType.COMMAND, message_for_test.get_message_type())
        self.assertEqual(MessageCommandType.TEST_CASE_SETUP, message_for_test.get_message_sub_type())
        self.assertEqual((message_counter + 1) % 256, message_for_test.get_message_token())
        self.assertDictEqual(tlv_dict_to_send, message_for_test.get_message_tlv_dict())
        
    def test_message_serialization_no_tlv(self):
        # Create the message
        message_for_test = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3)
        # Check serialization output
        output_result = message_for_test.serialize()
        expected_output = '4001030001020300'
        self.assertEqual(expected_output, output_result.hex())

    def test_message_serialization_with_tlv(self):
        # Create the message
        tlv_dict_to_send = {TlvKnownTags.TEST_REPORT: 'OK', TlvKnownTags.FAILURE_REASON: b'\x12\x34\x56'}
        message_for_test = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3, tlv_dict=tlv_dict_to_send)
        # Check serialization output
        output_result = message_for_test.serialize()
        expected_output = '40010300010203' + '09' + '6e' + '02' + '4f4b' + '70' + '03' + '123456'
        self.assertEqual(expected_output, output_result.hex())

    def test_parse_back_message_with_no_tlv(self):
        # Create raw message
        raw_message = b'\x40\x01\x03\x00\x01\x02\x03\x00'
        # Parse message back
        message = Message().parse_packet(raw_message)
        # Check content
        self.assertEqual(MessageType.COMMAND, message.get_message_type())
        self.assertEqual(MessageCommandType.TEST_CASE_SETUP, message.get_message_sub_type())
        self.assertEqual(1, message.get_message_token())
        self.assertEqual(None, message.get_message_tlv_dict())

    def test_parse_back_message_with_tlv(self):
        # Create raw message
        raw_message = b'\x40\x01\x03\x00\x01\x02\x03\x09\x6e\x02\x4f\x4b\x70\x03\x12\x34\x56'
        # Parse message back
        message = Message().parse_packet(raw_message)
        # Check content
        self.assertEqual(MessageType.COMMAND, message.get_message_type())
        self.assertEqual(MessageCommandType.TEST_CASE_SETUP, message.get_message_sub_type())
        self.assertEqual(1, message.get_message_token())
        self.assertDictEqual({TlvKnownTags.TEST_REPORT: b'\x4f\x4b', TlvKnownTags.FAILURE_REASON: b'\x12\x34\x56'}, message.get_message_tlv_dict())

    def test_ack_message_matching(self):
        # Create the messages
        message_sent = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3)
        raw_ack_message_received = b'\x60\x01\x00\x00\x01\x02\x03\x00'
        # Parse and compare
        message_received = Message().parse_packet(raw_ack_message_received)
        self.assertTrue(message_sent.check_if_ack_message_is_matching(message_received))

    def test_ack_message_gen_and_match(self):
        # Create the messages
        message_sent = Message(msg_type=MessageType.COMMAND, sub_type=MessageCommandType.TEST_CASE_SETUP, test_section=1, test_suite=2, test_case=3)
        ack_message_received = message_sent.generate_ack_message(MessageAckType.ACK)
        # Parse and compare
        self.assertTrue(message_sent.check_if_ack_message_is_matching(ack_message_received))

if __name__ == '__main__':
    # Start unittests
    unittest.main()