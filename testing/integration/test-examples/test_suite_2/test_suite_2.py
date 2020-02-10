import generic_test_case

@generic_test_case.define_test_case_parameters([], 0, 2, 1) # ([], test_section_id, test_suite_id, test_case_id)
class MyTest(generic_test_case.GenericTest.BasicTest):
    pass

@generic_test_case.define_test_case_parameters([], 0, 2, 2)
class MyTest2(generic_test_case.GenericTest.BasicTest):
    pass