# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\audre\workspace\S4e_Depart\S4e_Depart.Vitis\main_design_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\audre\workspace\S4e_Depart\S4e_Depart.Vitis\main_design_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {main_design_wrapper}\
-hw {C:\Users\audre\workspace\S4e_Depart\main_design_wrapper.xsa}\
-fsbl-target {psu_cortexa53_0} -out {C:/Users/audre/workspace/S4e_Depart/S4e_Depart.Vitis}

platform write
domain create -name {standalone_microblaze_0} -display-name {standalone_microblaze_0} -os {standalone} -proc {microblaze_0} -runtime {cpp} -arch {32-bit} -support-app {empty_application}
platform generate -domains 
platform active {main_design_wrapper}
domain active {zynq_fsbl}
domain active {standalone_microblaze_0}
platform generate -quick
platform generate
platform clean
platform generate
platform generate -domains 
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform clean
platform generate
platform clean
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {/mnt/home/gegi/usager/cles2601/Bureau/APP4-PROB/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform generate -domains 
platform clean
platform generate
platform clean
platform generate
platform active {main_design_wrapper}
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/design_1_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/XSA_TEST/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {C:/Users/Utilisateur/Documents/Pr_S4/app4/VIVADO/S4e_Depart_etud/main_design_wrapper.xsa}
platform clean
platform generate
platform generate -domains 
platform clean
platform generate
platform clean
platform generate
