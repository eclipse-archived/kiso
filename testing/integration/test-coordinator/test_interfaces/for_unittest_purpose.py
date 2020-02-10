# To use in real test-case:
import generic_test_case

@generic_test_case.define_test_case_parameters([], 1, 2, 3)
class MyTest(generic_test_case.GenericTest.BasicTest):
    def test_another_one(self):
        pass

@generic_test_case.define_test_case_parameters([], 1, 2, 4)
class MyTest2(generic_test_case.GenericTest.BasicTest):
    pass

