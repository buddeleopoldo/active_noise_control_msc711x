;
; Define configuration specific values
;



.provide _SR_Setting, 0xe4000c	    ; The value to set the SR after reset:
                                    ; exception mode
                                    ; interrupt level 7
                                    ; saturation on
                                    ; rounding mode: nearest even


.provide _ddrb, 0x20000000        ; beginning of SDRAM
.provide _ddre, _ddrb+0x00ffffff  ;add 16MB to the base address

.provide _M1_start, 0x0000
.provide _M1_size,  0x30000
.provide _M1_end, (_M1_start + _M1_size)

.provide _M2_start, 0x01000000   ; L2 heap start.
.provide _M2_size,  0x30000   ; L2 heap size.
.provide _M2_end, (_M2_start + _M2_size)


.provide _osvecb, 0x00000000        ; OS interrupt vectors
.provide _osvece, 0x00000fff
.provide _ocramb, _M1_start + 0x1000
									; Start of on chip RAM (really starts
                                    ;  at 0x00000000, but int vectors are
                                    ;  there.


.provide _size_of_M1_code_and_data, 0x20000


.provide _StackStart, _ocramb + _size_of_M1_code_and_data
.provide _TopOfStack, _M1_end


.memory _ddrb, _ddre, "rwx"     ; make the 16MB of SDRAM available
.memory _M1_start, _M1_end, "rwx"
.memory _M2_start, _M2_end, "rwx"

; Define a region for the stack
.reserve _StackStart, _TopOfStack   ; Reserve for stack space


.entry ___crt0_start

.xref _interr_vector_file ; in order to link to binary


.org _osvecb
.segment .osvecb, ".osvecb"


.org _ocramb
.segment .oskernel_text_run_time_critical, ".oskernel_text_run_time_critical"
.segment .data, ".oskernel_local_data", ".oskernel_bss", ".data",".ramsp_0",".default",".bss"

.org _M2_start
.segment .text, ".oskernel_text_run_time", ".text"
.segment .oskernel_shared_data, ".oskernel_shared_data"
.segment .os_shared_data, ".os_shared_data"
.segment .rotable, ".oskernel_rom", ".init_table"
.segment .roinit, ".oskernel_init", ".rom_init"

.org _ddrb
.segment .oskernel_text_initialization, ".oskernel_text_initialization"
.segment .oskernel_initialization_data, ".oskernel_initialization_data"
.segment .demo_ddr_data, ".demo_ddr_data"
