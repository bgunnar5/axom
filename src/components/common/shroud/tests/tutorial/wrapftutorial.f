! wrapftutorial.f
! This is generated code, do not edit
module tutorial_mod
    use fstr_mod
    use, intrinsic :: iso_c_binding, only : C_PTR
    implicit none
    
    ! splicer begin module_top
    ! splicer end module_top
    
    ! splicer begin class.Class1.module_top
    ! splicer end class.Class1.module_top
    
    type class1
        type(C_PTR) voidptr
        ! splicer begin class.Class1.component_part
        ! splicer end class.Class1.component_part
    contains
        procedure :: method1 => class1_method1
        ! splicer begin class.Class1.type_bound_procedure_part
        ! splicer end class.Class1.type_bound_procedure_part
    end type class1
    
    
    interface operator (.eq.)
        module procedure class1_eq
    end interface
    
    interface operator (.ne.)
        module procedure class1_ne
    end interface
    
    interface
        
        function tut_class1_new() result(rv) &
                bind(C, name="TUT_class1_new")
            use iso_c_binding
            implicit none
            type(C_PTR) :: rv
        end function tut_class1_new
        
        subroutine tut_class1_method1(self) &
                bind(C, name="TUT_class1_method1")
            use iso_c_binding
            implicit none
            type(C_PTR), value, intent(IN) :: self
        end subroutine tut_class1_method1
        
        ! splicer begin class.Class1.additional_interfaces
        ! splicer end class.Class1.additional_interfaces
        
        subroutine tut_function1() &
                bind(C, name="TUT_function1")
            use iso_c_binding
            implicit none
        end subroutine tut_function1
        
        function tut_function2(arg1, arg2) result(rv) &
                bind(C, name="TUT_function2")
            use iso_c_binding
            implicit none
            real(C_DOUBLE), value, intent(IN) :: arg1
            integer(C_INT), value, intent(IN) :: arg2
            real(C_DOUBLE) :: rv
        end function tut_function2
        
        function tut_function3(arg) result(rv) &
                bind(C, name="TUT_function3")
            use iso_c_binding
            implicit none
            logical(C_BOOL), value, intent(IN) :: arg
            logical(C_BOOL) :: rv
        end function tut_function3
        
        pure function tut_function4a(arg1, arg2) result(rv) &
                bind(C, name="TUT_function4a")
            use iso_c_binding
            implicit none
            character(kind=C_CHAR), intent(IN) :: arg1(*)
            character(kind=C_CHAR), intent(IN) :: arg2(*)
            type(C_PTR) rv
        end function tut_function4a
        
        function tut_function4b(arg1, arg2) result(rv) &
                bind(C, name="TUT_function4b")
            use iso_c_binding
            implicit none
            character(kind=C_CHAR), intent(IN) :: arg1(*)
            character(kind=C_CHAR), intent(IN) :: arg2(*)
            type(C_PTR) rv
        end function tut_function4b
        
        function tut_function5(arg1, arg2) result(rv) &
                bind(C, name="TUT_function5")
            use iso_c_binding
            implicit none
            real(C_DOUBLE), value, intent(IN) :: arg1
            integer(C_INT), value, intent(IN) :: arg2
            real(C_DOUBLE) :: rv
        end function tut_function5
        
        subroutine tut_function6_from_name(name) &
                bind(C, name="TUT_function6_from_name")
            use iso_c_binding
            implicit none
            character(kind=C_CHAR), intent(IN) :: name(*)
        end subroutine tut_function6_from_name
        
        subroutine tut_function6_from_index(indx) &
                bind(C, name="TUT_function6_from_index")
            use iso_c_binding
            implicit none
            integer(C_INT), value, intent(IN) :: indx
        end subroutine tut_function6_from_index
        
        function tut_function8_int() result(rv) &
                bind(C, name="TUT_function8_int")
            use iso_c_binding
            implicit none
            integer(C_INT) :: rv
        end function tut_function8_int
        
        function tut_function8_double() result(rv) &
                bind(C, name="TUT_function8_double")
            use iso_c_binding
            implicit none
            real(C_DOUBLE) :: rv
        end function tut_function8_double
    end interface
    
    interface function6
        module procedure function6_from_name
        module procedure function6_from_index
    end interface function6

contains
    
    function class1_new() result(rv)
        use iso_c_binding
        implicit none
        type(class1) :: rv
        ! splicer begin class.Class1.method.new
        rv%voidptr = tut_class1_new()
        ! splicer end class.Class1.method.new
    end function class1_new
    
    subroutine class1_method1(obj)
        use iso_c_binding
        implicit none
        class(class1) :: obj
        ! splicer begin class.Class1.method.method1
        call tut_class1_method1(obj%voidptr)
        ! splicer end class.Class1.method.method1
    end subroutine class1_method1
    
    ! splicer begin class.Class1.additional_functions
    ! splicer end class.Class1.additional_functions
    
    subroutine function1()
        use iso_c_binding
        implicit none
        ! splicer begin function1
        call tut_function1()
        ! splicer end function1
    end subroutine function1
    
    function function2(arg1, arg2) result(rv)
        use iso_c_binding
        implicit none
        real(C_DOUBLE) :: arg1
        integer(C_INT) :: arg2
        real(C_DOUBLE) :: rv
        ! splicer begin function2
        rv = tut_function2(  &
            arg1,  &
            arg2)
        ! splicer end function2
    end function function2
    
    function function3(arg) result(rv)
        use iso_c_binding
        implicit none
        logical :: arg
        logical :: rv
        ! splicer begin function3
        rv = booltological(tut_function3(logicaltobool(arg)))
        ! splicer end function3
    end function function3
    
    function function4a(arg1, arg2) result(rv)
        use iso_c_binding
        implicit none
        character(*) :: arg1
        character(*) :: arg2
        character(kind=C_CHAR, len=strlen_ptr(tut_function4a(trim(arg1) // C_NULL_CHAR, trim(arg2) // C_NULL_CHAR))) :: rv
        ! splicer begin function4a
        rv = fstr(tut_function4a(  &
            trim(arg1) // C_NULL_CHAR,  &
            trim(arg2) // C_NULL_CHAR))
        ! splicer end function4a
    end function function4a
    
    subroutine function4b(arg1, arg2, rv)
        use iso_c_binding
        implicit none
        character(*) :: arg1
        character(*) :: arg2
        character(*), intent(OUT) :: rv
        type(C_PTR) :: rv_ptr
        ! splicer begin function4b
        rv_ptr = tut_function4b(  &
            trim(arg1) // C_NULL_CHAR,  &
            trim(arg2) // C_NULL_CHAR)
        call FccCopyPtr(rv, len(rv), rv_ptr)
        ! splicer end function4b
    end subroutine function4b
    
    function function5(arg1, arg2) result(rv)
        use iso_c_binding
        implicit none
        real(C_DOUBLE), optional :: arg1
        integer(C_INT), optional :: arg2
        real(C_DOUBLE) :: rv
        if (.not. present(arg1)) then
            arg1 = 3.13
        endif
        if (.not. present(arg2)) then
            arg2 = 5
        endif
        ! splicer begin function5
        rv = tut_function5(  &
            arg1,  &
            arg2)
        ! splicer end function5
    end function function5
    
    subroutine function6_from_name(name)
        use iso_c_binding
        implicit none
        character(*) :: name
        ! splicer begin function6_from_name
        call tut_function6_from_name(trim(name) // C_NULL_CHAR)
        ! splicer end function6_from_name
    end subroutine function6_from_name
    
    subroutine function6_from_index(indx)
        use iso_c_binding
        implicit none
        integer(C_INT) :: indx
        ! splicer begin function6_from_index
        call tut_function6_from_index(indx)
        ! splicer end function6_from_index
    end subroutine function6_from_index
    
    function function8_int() result(rv)
        use iso_c_binding
        implicit none
        integer(C_INT) :: rv
        ! splicer begin function8_int
        rv = tut_function8_int()
        ! splicer end function8_int
    end function function8_int
    
    function function8_double() result(rv)
        use iso_c_binding
        implicit none
        real(C_DOUBLE) :: rv
        ! splicer begin function8_double
        rv = tut_function8_double()
        ! splicer end function8_double
    end function function8_double
    
    function class1_eq(a,b) result (rv)
        use iso_c_binding, only: c_associated
        implicit none
        type(class1), intent(IN) ::a,b
        logical :: rv
        if (c_associated(a%voidptr, b%voidptr)) then
            rv = .true.
        else
            rv = .false.
        endif
    end function class1_eq
    
    function class1_ne(a,b) result (rv)
        use iso_c_binding, only: c_associated
        implicit none
        type(class1), intent(IN) ::a,b
        logical :: rv
        if (.not. c_associated(a%voidptr, b%voidptr)) then
            rv = .true.
        else
            rv = .false.
        endif
    end function class1_ne

end module tutorial_mod
