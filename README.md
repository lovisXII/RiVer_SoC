# RISC-V-project
## I. Convention we took to name signals
Our signals are name according to the following convention :  

__**From_to_description_stage**__  

- **From** : designate the stage where the signal is coming from,  
- **to** : designate the stage into the one the signal goes,  
- **description** : a quick description of what the signal means,  
- **stage** : designate to which pipeline the signal belongs  

For the name of signals two case can occure :  

- **Upper case** : designate the input/ output of an entity
- **Lower case** : internal signals of an entity  

__example__ :  
    dec_to_exe_cin_stg1 : designate the signal carry in coming from decode and going to exe of the first pipeline.


