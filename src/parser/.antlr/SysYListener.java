// Generated from /mnt/f/test/src/parser/SysY.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link SysYParser}.
 */
public interface SysYListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link SysYParser#compUnit}.
	 * @param ctx the parse tree
	 */
	void enterCompUnit(SysYParser.CompUnitContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#compUnit}.
	 * @param ctx the parse tree
	 */
	void exitCompUnit(SysYParser.CompUnitContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#decl}.
	 * @param ctx the parse tree
	 */
	void enterDecl(SysYParser.DeclContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#decl}.
	 * @param ctx the parse tree
	 */
	void exitDecl(SysYParser.DeclContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#constDecl}.
	 * @param ctx the parse tree
	 */
	void enterConstDecl(SysYParser.ConstDeclContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#constDecl}.
	 * @param ctx the parse tree
	 */
	void exitConstDecl(SysYParser.ConstDeclContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#bType}.
	 * @param ctx the parse tree
	 */
	void enterBType(SysYParser.BTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#bType}.
	 * @param ctx the parse tree
	 */
	void exitBType(SysYParser.BTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#constDef}.
	 * @param ctx the parse tree
	 */
	void enterConstDef(SysYParser.ConstDefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#constDef}.
	 * @param ctx the parse tree
	 */
	void exitConstDef(SysYParser.ConstDefContext ctx);
	/**
	 * Enter a parse tree produced by the {@code scalarConstInitVal}
	 * labeled alternative in {@link SysYParser#constInitVal}.
	 * @param ctx the parse tree
	 */
	void enterScalarConstInitVal(SysYParser.ScalarConstInitValContext ctx);
	/**
	 * Exit a parse tree produced by the {@code scalarConstInitVal}
	 * labeled alternative in {@link SysYParser#constInitVal}.
	 * @param ctx the parse tree
	 */
	void exitScalarConstInitVal(SysYParser.ScalarConstInitValContext ctx);
	/**
	 * Enter a parse tree produced by the {@code listConstInitVal}
	 * labeled alternative in {@link SysYParser#constInitVal}.
	 * @param ctx the parse tree
	 */
	void enterListConstInitVal(SysYParser.ListConstInitValContext ctx);
	/**
	 * Exit a parse tree produced by the {@code listConstInitVal}
	 * labeled alternative in {@link SysYParser#constInitVal}.
	 * @param ctx the parse tree
	 */
	void exitListConstInitVal(SysYParser.ListConstInitValContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#varDecl}.
	 * @param ctx the parse tree
	 */
	void enterVarDecl(SysYParser.VarDeclContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#varDecl}.
	 * @param ctx the parse tree
	 */
	void exitVarDecl(SysYParser.VarDeclContext ctx);
	/**
	 * Enter a parse tree produced by the {@code uninitVarDef}
	 * labeled alternative in {@link SysYParser#varDef}.
	 * @param ctx the parse tree
	 */
	void enterUninitVarDef(SysYParser.UninitVarDefContext ctx);
	/**
	 * Exit a parse tree produced by the {@code uninitVarDef}
	 * labeled alternative in {@link SysYParser#varDef}.
	 * @param ctx the parse tree
	 */
	void exitUninitVarDef(SysYParser.UninitVarDefContext ctx);
	/**
	 * Enter a parse tree produced by the {@code initVarDef}
	 * labeled alternative in {@link SysYParser#varDef}.
	 * @param ctx the parse tree
	 */
	void enterInitVarDef(SysYParser.InitVarDefContext ctx);
	/**
	 * Exit a parse tree produced by the {@code initVarDef}
	 * labeled alternative in {@link SysYParser#varDef}.
	 * @param ctx the parse tree
	 */
	void exitInitVarDef(SysYParser.InitVarDefContext ctx);
	/**
	 * Enter a parse tree produced by the {@code scalarInitVal}
	 * labeled alternative in {@link SysYParser#initVal}.
	 * @param ctx the parse tree
	 */
	void enterScalarInitVal(SysYParser.ScalarInitValContext ctx);
	/**
	 * Exit a parse tree produced by the {@code scalarInitVal}
	 * labeled alternative in {@link SysYParser#initVal}.
	 * @param ctx the parse tree
	 */
	void exitScalarInitVal(SysYParser.ScalarInitValContext ctx);
	/**
	 * Enter a parse tree produced by the {@code listInitval}
	 * labeled alternative in {@link SysYParser#initVal}.
	 * @param ctx the parse tree
	 */
	void enterListInitval(SysYParser.ListInitvalContext ctx);
	/**
	 * Exit a parse tree produced by the {@code listInitval}
	 * labeled alternative in {@link SysYParser#initVal}.
	 * @param ctx the parse tree
	 */
	void exitListInitval(SysYParser.ListInitvalContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#funcDef}.
	 * @param ctx the parse tree
	 */
	void enterFuncDef(SysYParser.FuncDefContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#funcDef}.
	 * @param ctx the parse tree
	 */
	void exitFuncDef(SysYParser.FuncDefContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#funcType}.
	 * @param ctx the parse tree
	 */
	void enterFuncType(SysYParser.FuncTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#funcType}.
	 * @param ctx the parse tree
	 */
	void exitFuncType(SysYParser.FuncTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#funcFParams}.
	 * @param ctx the parse tree
	 */
	void enterFuncFParams(SysYParser.FuncFParamsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#funcFParams}.
	 * @param ctx the parse tree
	 */
	void exitFuncFParams(SysYParser.FuncFParamsContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#funcFParam}.
	 * @param ctx the parse tree
	 */
	void enterFuncFParam(SysYParser.FuncFParamContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#funcFParam}.
	 * @param ctx the parse tree
	 */
	void exitFuncFParam(SysYParser.FuncFParamContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(SysYParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(SysYParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#blockItem}.
	 * @param ctx the parse tree
	 */
	void enterBlockItem(SysYParser.BlockItemContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#blockItem}.
	 * @param ctx the parse tree
	 */
	void exitBlockItem(SysYParser.BlockItemContext ctx);
	/**
	 * Enter a parse tree produced by the {@code assignment}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterAssignment(SysYParser.AssignmentContext ctx);
	/**
	 * Exit a parse tree produced by the {@code assignment}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitAssignment(SysYParser.AssignmentContext ctx);
	/**
	 * Enter a parse tree produced by the {@code expStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterExpStmt(SysYParser.ExpStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code expStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitExpStmt(SysYParser.ExpStmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code blockStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterBlockStmt(SysYParser.BlockStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code blockStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitBlockStmt(SysYParser.BlockStmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code ifStmt1}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterIfStmt1(SysYParser.IfStmt1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code ifStmt1}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitIfStmt1(SysYParser.IfStmt1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code ifStmt2}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterIfStmt2(SysYParser.IfStmt2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code ifStmt2}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitIfStmt2(SysYParser.IfStmt2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code whileStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterWhileStmt(SysYParser.WhileStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code whileStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitWhileStmt(SysYParser.WhileStmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code breakStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterBreakStmt(SysYParser.BreakStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code breakStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitBreakStmt(SysYParser.BreakStmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code continueStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterContinueStmt(SysYParser.ContinueStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code continueStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitContinueStmt(SysYParser.ContinueStmtContext ctx);
	/**
	 * Enter a parse tree produced by the {@code returnStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturnStmt(SysYParser.ReturnStmtContext ctx);
	/**
	 * Exit a parse tree produced by the {@code returnStmt}
	 * labeled alternative in {@link SysYParser#stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturnStmt(SysYParser.ReturnStmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#exp}.
	 * @param ctx the parse tree
	 */
	void enterExp(SysYParser.ExpContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#exp}.
	 * @param ctx the parse tree
	 */
	void exitExp(SysYParser.ExpContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#cond}.
	 * @param ctx the parse tree
	 */
	void enterCond(SysYParser.CondContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#cond}.
	 * @param ctx the parse tree
	 */
	void exitCond(SysYParser.CondContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#lVal}.
	 * @param ctx the parse tree
	 */
	void enterLVal(SysYParser.LValContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#lVal}.
	 * @param ctx the parse tree
	 */
	void exitLVal(SysYParser.LValContext ctx);
	/**
	 * Enter a parse tree produced by the {@code primaryExp1}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void enterPrimaryExp1(SysYParser.PrimaryExp1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code primaryExp1}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void exitPrimaryExp1(SysYParser.PrimaryExp1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code primaryExp2}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void enterPrimaryExp2(SysYParser.PrimaryExp2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code primaryExp2}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void exitPrimaryExp2(SysYParser.PrimaryExp2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code primaryExp3}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void enterPrimaryExp3(SysYParser.PrimaryExp3Context ctx);
	/**
	 * Exit a parse tree produced by the {@code primaryExp3}
	 * labeled alternative in {@link SysYParser#primaryExp}.
	 * @param ctx the parse tree
	 */
	void exitPrimaryExp3(SysYParser.PrimaryExp3Context ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#number}.
	 * @param ctx the parse tree
	 */
	void enterNumber(SysYParser.NumberContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#number}.
	 * @param ctx the parse tree
	 */
	void exitNumber(SysYParser.NumberContext ctx);
	/**
	 * Enter a parse tree produced by the {@code unary1}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void enterUnary1(SysYParser.Unary1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code unary1}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void exitUnary1(SysYParser.Unary1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code unary2}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void enterUnary2(SysYParser.Unary2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code unary2}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void exitUnary2(SysYParser.Unary2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code unary3}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void enterUnary3(SysYParser.Unary3Context ctx);
	/**
	 * Exit a parse tree produced by the {@code unary3}
	 * labeled alternative in {@link SysYParser#unaryExp}.
	 * @param ctx the parse tree
	 */
	void exitUnary3(SysYParser.Unary3Context ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#unaryOp}.
	 * @param ctx the parse tree
	 */
	void enterUnaryOp(SysYParser.UnaryOpContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#unaryOp}.
	 * @param ctx the parse tree
	 */
	void exitUnaryOp(SysYParser.UnaryOpContext ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#funcRParams}.
	 * @param ctx the parse tree
	 */
	void enterFuncRParams(SysYParser.FuncRParamsContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#funcRParams}.
	 * @param ctx the parse tree
	 */
	void exitFuncRParams(SysYParser.FuncRParamsContext ctx);
	/**
	 * Enter a parse tree produced by the {@code expAsRParam}
	 * labeled alternative in {@link SysYParser#funcRParam}.
	 * @param ctx the parse tree
	 */
	void enterExpAsRParam(SysYParser.ExpAsRParamContext ctx);
	/**
	 * Exit a parse tree produced by the {@code expAsRParam}
	 * labeled alternative in {@link SysYParser#funcRParam}.
	 * @param ctx the parse tree
	 */
	void exitExpAsRParam(SysYParser.ExpAsRParamContext ctx);
	/**
	 * Enter a parse tree produced by the {@code stringAsRParam}
	 * labeled alternative in {@link SysYParser#funcRParam}.
	 * @param ctx the parse tree
	 */
	void enterStringAsRParam(SysYParser.StringAsRParamContext ctx);
	/**
	 * Exit a parse tree produced by the {@code stringAsRParam}
	 * labeled alternative in {@link SysYParser#funcRParam}.
	 * @param ctx the parse tree
	 */
	void exitStringAsRParam(SysYParser.StringAsRParamContext ctx);
	/**
	 * Enter a parse tree produced by the {@code mul2}
	 * labeled alternative in {@link SysYParser#mulExp}.
	 * @param ctx the parse tree
	 */
	void enterMul2(SysYParser.Mul2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code mul2}
	 * labeled alternative in {@link SysYParser#mulExp}.
	 * @param ctx the parse tree
	 */
	void exitMul2(SysYParser.Mul2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code mul1}
	 * labeled alternative in {@link SysYParser#mulExp}.
	 * @param ctx the parse tree
	 */
	void enterMul1(SysYParser.Mul1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code mul1}
	 * labeled alternative in {@link SysYParser#mulExp}.
	 * @param ctx the parse tree
	 */
	void exitMul1(SysYParser.Mul1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code add2}
	 * labeled alternative in {@link SysYParser#addExp}.
	 * @param ctx the parse tree
	 */
	void enterAdd2(SysYParser.Add2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code add2}
	 * labeled alternative in {@link SysYParser#addExp}.
	 * @param ctx the parse tree
	 */
	void exitAdd2(SysYParser.Add2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code add1}
	 * labeled alternative in {@link SysYParser#addExp}.
	 * @param ctx the parse tree
	 */
	void enterAdd1(SysYParser.Add1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code add1}
	 * labeled alternative in {@link SysYParser#addExp}.
	 * @param ctx the parse tree
	 */
	void exitAdd1(SysYParser.Add1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code rel2}
	 * labeled alternative in {@link SysYParser#relExp}.
	 * @param ctx the parse tree
	 */
	void enterRel2(SysYParser.Rel2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code rel2}
	 * labeled alternative in {@link SysYParser#relExp}.
	 * @param ctx the parse tree
	 */
	void exitRel2(SysYParser.Rel2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code rel1}
	 * labeled alternative in {@link SysYParser#relExp}.
	 * @param ctx the parse tree
	 */
	void enterRel1(SysYParser.Rel1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code rel1}
	 * labeled alternative in {@link SysYParser#relExp}.
	 * @param ctx the parse tree
	 */
	void exitRel1(SysYParser.Rel1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code eq1}
	 * labeled alternative in {@link SysYParser#eqExp}.
	 * @param ctx the parse tree
	 */
	void enterEq1(SysYParser.Eq1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code eq1}
	 * labeled alternative in {@link SysYParser#eqExp}.
	 * @param ctx the parse tree
	 */
	void exitEq1(SysYParser.Eq1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code eq2}
	 * labeled alternative in {@link SysYParser#eqExp}.
	 * @param ctx the parse tree
	 */
	void enterEq2(SysYParser.Eq2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code eq2}
	 * labeled alternative in {@link SysYParser#eqExp}.
	 * @param ctx the parse tree
	 */
	void exitEq2(SysYParser.Eq2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code lAnd2}
	 * labeled alternative in {@link SysYParser#lAndExp}.
	 * @param ctx the parse tree
	 */
	void enterLAnd2(SysYParser.LAnd2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code lAnd2}
	 * labeled alternative in {@link SysYParser#lAndExp}.
	 * @param ctx the parse tree
	 */
	void exitLAnd2(SysYParser.LAnd2Context ctx);
	/**
	 * Enter a parse tree produced by the {@code lAnd1}
	 * labeled alternative in {@link SysYParser#lAndExp}.
	 * @param ctx the parse tree
	 */
	void enterLAnd1(SysYParser.LAnd1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code lAnd1}
	 * labeled alternative in {@link SysYParser#lAndExp}.
	 * @param ctx the parse tree
	 */
	void exitLAnd1(SysYParser.LAnd1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code lOr1}
	 * labeled alternative in {@link SysYParser#lOrExp}.
	 * @param ctx the parse tree
	 */
	void enterLOr1(SysYParser.LOr1Context ctx);
	/**
	 * Exit a parse tree produced by the {@code lOr1}
	 * labeled alternative in {@link SysYParser#lOrExp}.
	 * @param ctx the parse tree
	 */
	void exitLOr1(SysYParser.LOr1Context ctx);
	/**
	 * Enter a parse tree produced by the {@code lOr2}
	 * labeled alternative in {@link SysYParser#lOrExp}.
	 * @param ctx the parse tree
	 */
	void enterLOr2(SysYParser.LOr2Context ctx);
	/**
	 * Exit a parse tree produced by the {@code lOr2}
	 * labeled alternative in {@link SysYParser#lOrExp}.
	 * @param ctx the parse tree
	 */
	void exitLOr2(SysYParser.LOr2Context ctx);
	/**
	 * Enter a parse tree produced by {@link SysYParser#constExp}.
	 * @param ctx the parse tree
	 */
	void enterConstExp(SysYParser.ConstExpContext ctx);
	/**
	 * Exit a parse tree produced by {@link SysYParser#constExp}.
	 * @param ctx the parse tree
	 */
	void exitConstExp(SysYParser.ConstExpContext ctx);
}