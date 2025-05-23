const char * schema_text =
"// Copyright Sophgo Technologies Inc.\n"
"// Written by:\n"
"//     Pengchao Hu <pengchao.hu@sophgo.com>\n"
"// Created Time: 2018-12-07 15:34\n"
"// Description: this file is model definition file.\n"
"//\n"
"// Rules:\n"
"// 1. Don't remove any item, or change any type or id number. If must, you can only change item name.\n"
"// 2. You can add new item, but don't set it as required.\n"
"//    If new item is a Vector, check whether it is NULL first and then access its elements in bmruntime.\n"
"// 3. If this file changed, gen_model_header.sh need to be called manually to update model header files\n"
"//\n"
"// If you have any doubts, you can contact me (pengchao.hu@sophgo.com).\n"
"\n"
"namespace bmodel;\n"
"\n"
"// to store binary data\n"
"struct Binary {\n"
"start:uint64;\n"
"size:uint64;\n"
"}\n"
"\n"
"table Shape {\n"
"dim:[uint64] (id:0);\n"
"}\n"
"\n"
"table CmdGroup {\n"
"bdc_num:uint32 (id: 0);       // m_bdc_group_id_v\n"
"gdma_num:uint32 (id: 1);      // m_gdma_group_id_v\n"
"binary_bdc:Binary (id: 2);    // bdc binary data\n"
"binary_gdma:Binary (id: 3);   // gdma binary data\n"
"bdc_cmd_byte:uint32 (id: 4);  // m_bdc_cmd_byte_v\n"
"gdma_cmd_byte:uint32 (id: 5); // m_gdma_cmd_byte_v\n"
"}\n"
"\n"
"table CoreCommands {\n"
"gdma_tiu_commands:[CmdGroup] (id: 0);  /* tpu command list with multiple id sync groups  */\n"
"sdma_commands: [Binary] (id: 1);       /* sdma command list */\n"
"hau_commands: [Binary] (id: 2);        /* hau command list */\n"
"cdma_commands: [Binary] (id: 3);       /* 6x cdma command list for each direction */\n"
"}\n"
"\n"
"table StageIR {\n"
"ir_info_len:uint32 (id: 0);  // ir_info_len_vv\n"
"height_high:int32 (id: 1);   // stage_param_vv\n"
"height_low:int32 (id: 2);\n"
"width_high:int32 (id: 3);\n"
"width_low:int32 (id: 4);\n"
"}\n"
"\n"
"table Location {\n"
"name:string (required, id:0);   // operation name\n"
"offset:uint64 (id:1);           // offset in binary_coeff\n"
"size:uint64 (id:2);             // coeff size of operation\n"
"}\n"
"\n"
"table CoeffMem {\n"
"address:uint64 (id: 0);\n"
"check_code:[uint8] (id: 1);  // sha256 for check binary\n"
"binary_coeff:Binary (id: 2); // coeff binary data\n"
"location:[Location] (id: 3); // each location in binary for coeff of operations\n"
"encrypt_mode:int32 (id: 4);  // 0: no encrypt; 1: encrypt by customer\n"
"decrypt_size:uint64 (id: 5); // coeff size after decrypt\n"
"}\n"
"\n"
"table Tensor {\n"
"name:string (required, id: 0); // tensor name\n"
"data_type:uint32 (id: 1);      // tensor data type\n"
"gmem_stmode:int32 (id: 2);     // gmem stmode\n"
"device_addr:uint64 (id: 3);    // input/output_tensor_mem_map_v\n"
"size:uint64 (id:4);            // tensor device mem size\n"
"shape:[Shape] (id: 5);         // for static net, only one shape\n"
"// for dynamic net, each shape for each stage\n"
"mem_type:uint32 (id: 6);       // 0 : used in tpu,\n"
"// 1 : used in cpu,\n"
"// 2 : used in both cpu/tpu layer\n"
"scale:float32 = 1.0 (id: 7);\n"
"cpu_addr:uint32 (id: 8);       // tensor cpu mem. for cpu layer, recode every tensor's offset\n"
"pad_h:uint32 (id: 9);          // this item is for conv 3ic(hack for BM1684 to improve efficiency): higher 16bit: pad_h_top, lower 16bit: pad_h_down\n"
"zero_point:int32 = 0 (id: 10); // zero point for requant or dequant\n"
"hidden:int32 (id: 11);         // hidden tensor, for cascade model. 0:hidden;1:input;2:output\n"
"index:int32 (id: 12);          // input or output index\n"
"}\n"
"\n"
"table CpuConst {\n"
"name:string (id: 0);           // cpu const name\n"
"const_data:Binary (id: 1);     // coeff memory\n"
"check_code:[uint8] (id: 2);    // sha256 for check binary\n"
"}\n"
"\n"
"table CpuParam {\n"
"op_type:int32 (id: 0);                    /* cpu layer op type  */\n"
"binary_param:Binary (id: 1);              /* cpu layer paramter */\n"
"cpu_const:[CpuConst] (id: 2);\n"
"is_custom:int32 (id: 3);                  /* is cpu layer custom */\n"
"}\n"
"\n"
"table OutputFrom {\n"
"indice:[int32] (id: 0);\n"
"}\n"
"\n"
"table MergeParam {\n"
"output_from: [OutputFrom] (id: 0);        /* len(output_from)==len(output_tensor), */\n"
"}\n"
"\n"
"table SwitchParam {\n"
"output_from: [int32] (id: 0);         /* len(output_from)==len(output_tensor), */\n"
"output_branch: [int32] (id: 1);       /* 0: false branch, 1: true branch */\n"
"}\n"
"\n"
"table SubNet {\n"
"subnet_mode:int32 (id: 0);                 /* tpu:0 cpu:1 */\n"
"cmd_group:[CmdGroup] (id: 1);              /* tpu instruction   */\n"
"cpu_param:[CpuParam] (id: 2);              /* cpu parameter     */\n"
"input_tensor:[Tensor] (id: 3);\n"
"output_tensor:[Tensor] (id: 4);\n"
"is_dynamic:int32 (id: 5);                  /* dynamic flag to support mix static/dynamic subnet */\n"
"ir_offset:uint32 (id: 6);                  /* per subnet ir offset */\n"
"ir_len:uint32 (id: 7);                     /* per subnet ir length */\n"
"n_dynamic:int32 (id: 8);                   /* per subnet n_dynamic */\n"
"h_w_dynamic:int32 (id: 9);                 /* per subnet h_w_dynamic */\n"
"id:int32 = -1 (id: 10);                    /* subnet index */\n"
"next_subnet_ids:[int32] (id: 11);          /* next subnet ids for running: -1 means invalid branch, empty means end */\n"
"merge_param: MergeParam (id: 12);\n"
"switch_param: SwitchParam (id: 13);\n"
"core_commands: [CoreCommands] (id: 14);\n"
"}\n"
"\n"
"table NetStatic {      // for old bmodel, not use any more\n"
"input_tensor:[Tensor] (required, id: 0);\n"
"output_tensor:[Tensor] (required, id: 1);\n"
"cmd_group:[CmdGroup] (id: 2);\n"
"ctx_addr:uint64 (id: 3);\n"
"ctx_size:uint64 (id: 4);\n"
"coeff_mem:CoeffMem (id: 5);\n"
"sub_net:[SubNet] (id: 6);\n"
"net_profile:Binary (id: 7);\n"
"}\n"
"\n"
"table NetDynamic {    // for old bmodel, not use any more\n"
"input_tensor:[Tensor] (required, id: 0);\n"
"output_tensor:[Tensor] (required, id: 1);\n"
"n_dynamic:int32 (id: 2); // n_dynamic\n"
"h_w_dynamic:int32 (id: 3); // h_w_dynamic\n"
"stage_ir:[StageIR] (id: 4);\n"
"binary_ir:Binary (id: 5); // ir binary data\n"
"ctx_addr:uint64 (id: 6);\n"
"ctx_size:uint64 (id: 7);\n"
"coeff_mem:CoeffMem (id: 8);\n"
"sub_net:[SubNet] (id: 9);\n"
"}\n"
"\n"
"table NetParameter {\n"
"input_tensor:[Tensor] (required, id: 0);   /* per net input  tensor */\n"
"output_tensor:[Tensor] (required, id: 1);  /* per net output tensor */\n"
"ctx_addr:uint64 (id: 2);                   /* per net neuron tensor device memory */\n"
"ctx_size:uint64 (id: 3);\n"
"coeff_mem:CoeffMem (id: 4);                /* per net coeff tensor device memory */\n"
"is_dynamic:int32 (id: 5);                  /* whether net input shape varify */\n"
"n_dynamic:int32 (id: 6);                   /* whether net input batch varify */\n"
"h_w_dynamic:int32 (id: 7);                 /* whether net input h/w   varify */\n"
"cmd_group:[CmdGroup] (id: 8);              /* static tpu subnets share gdma/bdc binary context */\n"
"net_profile:Binary (id: 9);\n"
"stage_ir:[StageIR] (id: 10);\n"
"binary_ir:Binary (id: 11);                 /* dynamic tpu subnet share ir binary context */\n"
"sub_net:[SubNet] (id: 12);                 /* per subnet info */\n"
"cpu_mem_size:uint32 (id: 13);              /* per net just alloc one max cpu mem size */\n"
"\n"
"// Per group neuron tensor device memory sizes.\n"
"// If this array exists, it will be used instead of ctx_size\n"
"ctx_sizes:[uint64] (id: 14);\n"
"\n"
"// record net stat_gen info for simulate, optional\n"
"net_stat:Binary (id: 15);\n"
"\n"
"// The net may be deployed on multi cores\n"
"core_num: uint32 (id: 16);\n"
"\n"
"// io address alone\n"
"io_addr:uint64 (id: 17);\n"
"io_size:uint64 (id: 18);\n"
"tensor_loc:Binary (id: 19);\n"
"\n"
"// dynamic combine\n"
"dynamic_ctx_addr: uint64 (id: 20);        // dynamic ir ctx_addr\n"
"dynamic_coeff_offset: uint64 (id: 21);\n"
"dynamic_combined_coeff_offset: uint64 (id: 22);  // dynamic_coeff_offset if bmodel combined\n"
"}\n"
"\n"
"table Cascade {\n"
"device_id:uint32  (id: 0);        // which device to run\n"
"step:uint32       (id: 1);        // step to run: if multi, run in parallel; if none, end\n"
"main_name:string  (id: 2);        // net belong to\n"
"}\n"
"\n"
"table Net {\n"
"name:string (required, id: 0);     // net name\n"
"net_static:[NetStatic] (id: 1);    // for old bmodel, not use any more\n"
"net_dynamic:[NetDynamic] (id: 2);  // for old bmodel, not use any more\n"
"parameter:[NetParameter] (id: 3);  // one net can one or more stages\n"
"cascade:Cascade (id: 4);           // may run in multi devices or multi steps\n"
"addr_mode:int32 (id: 5);           // 0: basic mode, io and neuron mem alloc together by runtime\n"
"// 1: io alone mode, io mem and neuron mem alloc seperated by runtime\n"
"}\n"
"\n"
"table KernelModule {\n"
"file_name:string (required, id: 0);\n"
"binary:Binary (required, id: 1);\n"
"}\n"
"\n"
"table CpuopModule {\n"
"file_name:string (required, id: 0);\n"
"binary:Binary (required, id: 1);\n"
"}\n"
"\n"
"table Model {\n"
"type:string (required, id: 0);\n"
"version:string (required, id: 1);\n"
"time:string (required, id: 2);\n"
"chip:string (required, id: 3); // BM1682/BM1684/...\n"
"net:[Net] (required, id: 4);\n"
"neuron_size:uint64 (id: 5);    // max neuron size\n"
"kernel_module:KernelModule (id: 6);\n"
"device_num:uint32 (id: 7);     // The model may be run in multi devices\n"
"cpuop_module:CpuopModule (id: 8);\n"
"bmodel_type:uint32 (id: 9);    // 0 : coeff not combine\n"
"// 1 : coeff has been combine\n"
"}\n"
"\n"
"root_type Model;\n"
"";
