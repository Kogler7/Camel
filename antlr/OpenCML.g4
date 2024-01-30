grammar OpenCML;
import OpenCMLLex;

program : stmtList? EOF;

stmtList : stmt (SEP stmt)* SEP? ;

stmt : letStmt
     | useStmt
     | funcDef
     | typeDef
     | enumDef
     | retStmt
     | exprStmt
     | letSimple
     | useSimple
     ;

letStmt : LET carrier (':' type)? '='? entityExpr ;
useStmt : USE carrier '='? entityExpr ;
letSimple : carrier (':' type)? ':=' entityExpr ;
useSimple : carrier '::' entityExpr ;
withDef : WITH (entityRef | withDecl) ;
funcDef : annotations withDef? modifiers FUNC identRef paramDef ('->' type)? '='? stmtPack ;
typeDef : TYPE identRef '='? (type | typePack) ;
enumDef : ENUM identRef '='? dictUnpack ;
retStmt : RETURN entityExpr? ;
exprStmt : annotations entityExpr ;

carrier : identRef
        | listUnpack
        | dictUnpack
        ;

biasAnno : '(' INTEGER ',' INTEGER ')' ;
sizeAnno : '[' INTEGER ',' INTEGER ']' ;
annotation : '@' (identRef | dictPack | biasAnno | sizeAnno) ;
annotations : (annotation SEP?)* ;
modifiers : (INNER | OUTER | SYNC | SCOPED | STATIC | ATOMIC)* ;
withList : '<' argument (','  argument)* '>' ;
withDecl : '<' keyValDecl (',' keyValDecl)* '>' ;
paramDef : '(' (keyValDecl (',' keyValDecl)*)? ')' ;
argsList : '(' (argument (',' argument)*)? ')' ;
argument : identRef | entity
         | keyValExpr | entityExpr
         ;

typePack : '{' (keyValDecl (',' keyValDecl)*)? '}' ;
keyValDecl : identRef (annotation)? ':' nullableType ('=' entityExpr)? ;
keyValExpr : identRef (annotation)? '=' entityExpr ;

entityRef : identRef ('.' (INTEGER | identRef))* annotation? ;
functorRef: identRef (withList)? annotation? ;
listUnpack : '[' (identRef (',' identRef)*)? ']' ;
dictUnpack : '{' (identRef (',' identRef)*)? '}' ;
dictPack : '{' (keyValExpr (',' keyValExpr)*)? '}' ;
listPack : '[' (entityExpr (',' entityExpr)*)? ']' ;
stmtPack : '{' stmtList?  '}' ;
lambda   : (paramDef '=>')? stmtPack ;

entityExpr : (entity | normCall | linkCall | entityChain) (AS type)? ;
entityChain : (identRef | entity | linkCall)+ ;
entity : ((identRef | literal | listPack | dictPack | lambda) annotation?) | entityRef | functorRef ;
normCall : functorRef argsList ;
linkCall : linkCall '->' (identRef | functorRef | entity | normCall)
         | (identRef | entityRef | entity | functorRef | normCall)
           '->' (identRef | functorRef | entity | normCall)
         | identRef | entityRef | entity | functorRef | normCall
         ;


literal : value
        | STRING
        | MULTI_STR
        | FSTRING
        | NULL
        | TRUE
        | FALSE
        ;
value : (INTEGER | REAL) UNIT? ;


type : innerType
     | identRef
     | ANY_TYPE
     ;

innerType : NUMBER_TYPE
          | STRING_TYPE
          | BOOLEAN_TYPE
          | FUNCTOR_TYPE
          | BLOCK_TYPE
          | numberType
          | structType
          ;
numberType : scalarType
           | vectorType
           ;
scalarType : INTEGER_TYPE
           | REAL_TYPE
           | COMPLEX_TYPE
           ;
vectorType : ARRAY_TYPE ('<' scalarType '>')? ('[' INTEGER ']')?
           | MATRIX_TYPE ('<' scalarType '>')? ('[' INTEGER']')*
              ;
structType : LIST_TYPE ('<' nullableType (',' nullableType)* '>')? ('[' INTEGER ']')?
           | DICT_TYPE ('<' type ',' nullableType '>')?
           ;
nullableType : type '?'? ;

identRef : IDENTIFIER ;
