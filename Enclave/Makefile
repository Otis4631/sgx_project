cpp_flags = -g -std=c++11 -fopenmp 

Enclave_Cpp_Files := e_tools.cpp e_mat.cpp e_layer.cpp e_model.cpp test.cpp  
Enclave_Cpp_Objects := $(sort $(Enclave_Cpp_Files:.cpp=.o))


./%.o: ./%.cpp 
	@$(CXX) $(cpp_flags) -c $< -o $@
	@echo "generated  $@ using $<"


test_run: $(Enclave_Cpp_Objects)
	@echo "going to build $@ using $(Enclave_Cpp_Objects)"
	@$(CXX) $(cpp_flags) $^ -o $@ 
	@echo "LINK =>  $@"


clean:
	rm -rf *.o test_run