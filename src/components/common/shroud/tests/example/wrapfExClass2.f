! blah blah
! yada yada
!
module exclass2_mod
    use fstr_mod
    use iso_c_binding
    
    type exclass2
        type(C_PTR) obj
    contains
        procedure :: get_name => exclass2_get_name
        procedure :: get_name_length => exclass2_get_name_length
    end type exclass2
    
    interface
        
        function aa_exclass2_ex_class2(name) result(rv) bind(C, name="AA_exclass2_ex_class2")
            use iso_c_binding
            implicit none
            character(kind=C_CHAR) :: name(*)
            type(C_PTR) :: rv
        end function aa_exclass2_ex_class2
        
        subroutine aa_exclass2_ex_class1(self) bind(C, name="AA_exclass2_ex_class1")
            use iso_c_binding
            implicit none
            type(C_PTR), value :: self
        end subroutine aa_exclass2_ex_class1
        
        pure function aa_exclass2_get_name(self) result(rv) bind(C, name="AA_exclass2_get_name")
            use iso_c_binding
            implicit none
            type(C_PTR), value :: self
            type(C_PTR) rv
        end function aa_exclass2_get_name
        
        pure function aa_exclass2_get_name_length(self) result(rv) bind(C, name="AA_exclass2_get_name_length")
            use iso_c_binding
            implicit none
            type(C_PTR), value :: self
            integer(C_INT) :: rv
        end function aa_exclass2_get_name_length
    end interface

contains
    
    function exclass2_ex_class2(name) result(rv)
        implicit none
        character(*) :: name
        type(exclass2) :: rv
        ! splicer begin
        rv%obj = aa_exclass2_ex_class2(trim(name) // C_NULL_CHAR)
        ! splicer end
    end function exclass2_ex_class2
    
    subroutine exclass2_ex_class1(obj)
        implicit none
        type(exclass2) :: obj
        ! splicer begin
        call aa_exclass2_ex_class1(obj%obj)
        obj%obj = C_NULL_PTR
        ! splicer end
    end subroutine exclass2_ex_class1
    
    function exclass2_get_name(obj) result(rv)
        implicit none
        class(exclass2) :: obj
        character(kind=C_CHAR, len=aa_exclass2_get_name_length(obj%obj)) :: rv
        type(C_PTR) :: rv_ptr
        ! splicer begin
        rv = fstr(aa_exclass2_get_name(obj%obj))
        ! splicer end
    end function exclass2_get_name
    
    function exclass2_get_name_length(obj) result(rv)
        implicit none
        class(exclass2) :: obj
        integer(C_INT) :: rv
        ! splicer begin
        rv = aa_exclass2_get_name_length(obj%obj)
        ! splicer end
    end function exclass2_get_name_length

end module exclass2_mod
