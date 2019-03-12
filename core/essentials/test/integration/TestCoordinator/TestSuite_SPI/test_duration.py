"""
:module: test_duration
:platform: Windows
:synopsis: Test duration used for testing

:Copyright: (C) Bosch Connected Devices and Solutions. All Rights Reserved. Confidential.
"""
import random


class TestDuration(object):
    """ Class used to generate some test durations for testing purpose """
    def __init__(self):
        # a sequence of test duration that looks like: 1, 2, ..., 9, 10, 20, ..., 90, 100, 200, ... 900
        self.duration_sequence = [x for s in [10, 100, 1000] for x in range(0, 10*s, s) if x != 0]
        self.index_in_sequence = 0

    def get_next_test_duration(self):
        """
        :return: next test duration within the sequence
        """
        duration = self.duration_sequence[self.index_in_sequence]
        self.index_in_sequence = (self.index_in_sequence + 1) % len(self.duration_sequence)
        return duration

    def get_next_3_test_durations(self):
        """
        :return: shuffled next 3 test durations in sequence
        """
        duration1 = self.duration_sequence[self.index_in_sequence]
        self.index_in_sequence = (self.index_in_sequence + 1) % len(self.duration_sequence)
        duration2 = self.duration_sequence[self.index_in_sequence]
        self.index_in_sequence = (self.index_in_sequence + 1) % len(self.duration_sequence)
        duration3 = self.duration_sequence[self.index_in_sequence]
        self.index_in_sequence = (self.index_in_sequence + 1) % len(self.duration_sequence)

        durations = [duration1, duration2, duration3]
        random.shuffle(durations)
        return durations

    def get_random_test_duration(self):
        """
        :return: some random test duration within the sequence
        """
        index = random.randint(0, len(self.duration_sequence) - 1)
        return self.duration_sequence[index]

# singleton of test durations
test_durations = TestDuration()
