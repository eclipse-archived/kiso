import generic_test_case
import logging

@generic_test_case.define_test_case_parameters([], 0, 1, 1) # ([], test_section_id, test_suite_id, test_case_id)
class MyTest(generic_test_case.GenericTest.BasicTest):
    pass

@generic_test_case.define_test_case_parameters([], 0, 1, 2)
class MyTest2(generic_test_case.GenericTest.BasicTest):
    pass

@generic_test_case.define_test_case_parameters([], 0, 1, 3)
class MyTest3(generic_test_case.GenericTest.BasicTest):
    def test_run(self):
        logging.info("I HAVE RUN 0.1.3!")
