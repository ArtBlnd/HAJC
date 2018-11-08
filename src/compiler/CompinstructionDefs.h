#ifndef DEF_INST
#error DEF_INST is undefined.
#endif

//       Name | Capital Name | Id  | Flags            | Max Operands
DEF_INST(Move , MOVE         , 0x01, IT_FLAG::ITF_OP_2, 2)
DEF_INST(Add  , ADD          , 0x02, IT_FLAG::ITF_OP_2, 2)
DEF_INST(Sub  , SUB          , 0x03, IT_FLAG::ITF_OP_2, 2)
DEF_INST(Mul  , MUL          , 0x04, IT_FLAG::ITF_OP_2, 2)
DEF_INST(Div  , DIV          , 0x05, IT_FLAG::ITF_OP_2, 2)

DEF_INST(Push , PUSH         , 0x06, IT_FLAG::ITF_OP_1, 1)
DEF_INST(Pop  , POP          , 0x07, IT_FLAG::ITF_OP_1, 1)
DEF_INST(Call , CALL         , 0x08, IT_FLAG::ITF_OP_1, 1)

DEF_INST(Index, INDEX        , 0x09, IT_FLAG::ITF_OP_N, 6)
DEF_INST(If   , IF           , 0x0A, IT_FLAG::ITF_OP_3, 3)
DEF_INST(Label, LABEL        , 0x0B, IT_FLAG::ITF_OP_1, 1)

#undef DEF_INST