// Generated from /mnt/f/test/src/parser/CommonLex.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class CommonLex extends Lexer {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Int=1, Float=2, Void=3, Const=4, Return=5, If=6, Else=7, For=8, While=9, 
		Do=10, Break=11, Continue=12, Lparen=13, Rparen=14, Lbrkt=15, Rbrkt=16, 
		Lbrace=17, Rbrace=18, Comma=19, Semicolon=20, Question=21, Colon=22, Minus=23, 
		Exclamation=24, Tilde=25, Addition=26, Multiplication=27, Division=28, 
		Modulo=29, LAND=30, LOR=31, EQ=32, NEQ=33, LT=34, LE=35, GT=36, GE=37, 
		IntLiteral=38, HexadecimalDigits=39, ExponentPart=40, FloatLiteral=41, 
		Identifier=42, STRING=43, WS=44, LINE_COMMENT=45, COMMENT=46;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"Int", "Float", "Void", "Const", "Return", "If", "Else", "For", "While", 
			"Do", "Break", "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", 
			"Rbrace", "Comma", "Semicolon", "Question", "Colon", "Minus", "Exclamation", 
			"Tilde", "Addition", "Multiplication", "Division", "Modulo", "LAND", 
			"LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", "IntLiteral", "HexadecimalDigits", 
			"ExponentPart", "FractionPart", "FloatLiteral", "Identifier", "STRING", 
			"ESC", "WS", "LINE_COMMENT", "COMMENT"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'float'", "'void'", "'const'", "'return'", "'if'", "'else'", 
			"'for'", "'while'", "'do'", "'break'", "'continue'", "'('", "')'", "'['", 
			"']'", "'{'", "'}'", "','", "';'", "'?'", "':'", "'-'", "'!'", "'~'", 
			"'+'", "'*'", "'/'", "'%'", "'&&'", "'||'", "'=='", "'!='", "'<'", "'<='", 
			"'>'", "'>='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "Int", "Float", "Void", "Const", "Return", "If", "Else", "For", 
			"While", "Do", "Break", "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", 
			"Lbrace", "Rbrace", "Comma", "Semicolon", "Question", "Colon", "Minus", 
			"Exclamation", "Tilde", "Addition", "Multiplication", "Division", "Modulo", 
			"LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", "IntLiteral", "HexadecimalDigits", 
			"ExponentPart", "FloatLiteral", "Identifier", "STRING", "WS", "LINE_COMMENT", 
			"COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public CommonLex(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "CommonLex.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\60\u016f\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31"+
		"\t\31\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t"+
		" \4!\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t"+
		"+\4,\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\3\2\3\2\3\2\3\2\3\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3"+
		"\6\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\n"+
		"\3\n\3\n\3\n\3\n\3\n\3\13\3\13\3\13\3\f\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3"+
		"\r\3\r\3\r\3\r\3\r\3\r\3\r\3\16\3\16\3\17\3\17\3\20\3\20\3\21\3\21\3\22"+
		"\3\22\3\23\3\23\3\24\3\24\3\25\3\25\3\26\3\26\3\27\3\27\3\30\3\30\3\31"+
		"\3\31\3\32\3\32\3\33\3\33\3\34\3\34\3\35\3\35\3\36\3\36\3\37\3\37\3\37"+
		"\3 \3 \3 \3!\3!\3!\3\"\3\"\3\"\3#\3#\3$\3$\3$\3%\3%\3&\3&\3&\3\'\6\'\u00dd"+
		"\n\'\r\'\16\'\u00de\3\'\3\'\3\'\3\'\6\'\u00e5\n\'\r\'\16\'\u00e6\3\'\3"+
		"\'\3\'\3\'\6\'\u00ed\n\'\r\'\16\'\u00ee\5\'\u00f1\n\'\3(\3(\3(\3(\6(\u00f7"+
		"\n(\r(\16(\u00f8\3(\3(\3(\3(\6(\u00ff\n(\r(\16(\u0100\5(\u0103\n(\3)\3"+
		")\3)\6)\u0108\n)\r)\16)\u0109\3*\6*\u010d\n*\r*\16*\u010e\3*\3*\7*\u0113"+
		"\n*\f*\16*\u0116\13*\3*\5*\u0119\n*\3*\3*\6*\u011d\n*\r*\16*\u011e\3*"+
		"\5*\u0122\n*\3*\6*\u0125\n*\r*\16*\u0126\3*\5*\u012a\n*\3+\3+\5+\u012e"+
		"\n+\3+\3+\3+\3+\3+\5+\u0135\n+\3,\3,\7,\u0139\n,\f,\16,\u013c\13,\3-\3"+
		"-\3-\7-\u0141\n-\f-\16-\u0144\13-\3-\3-\3.\3.\3.\3.\5.\u014c\n.\3/\3/"+
		"\3/\3/\3\60\3\60\3\60\3\60\7\60\u0156\n\60\f\60\16\60\u0159\13\60\3\60"+
		"\5\60\u015c\n\60\3\60\3\60\3\60\3\60\3\61\3\61\3\61\3\61\7\61\u0166\n"+
		"\61\f\61\16\61\u0169\13\61\3\61\3\61\3\61\3\61\3\61\5\u0142\u0157\u0167"+
		"\2\62\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35"+
		"\20\37\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36"+
		";\37= ?!A\"C#E$G%I&K\'M(O)Q*S\2U+W,Y-[\2]._/a\60\3\2\t\3\2\62;\5\2\62"+
		";CHch\4\2GGgg\4\2--//\5\2C\\aac|\6\2\62;C\\aac|\5\2\13\f\17\17\"\"\2\u0187"+
		"\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2"+
		"\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2"+
		"\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2"+
		"\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2"+
		"\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3"+
		"\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2"+
		"\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2Q\3\2\2\2\2U\3\2\2\2\2"+
		"W\3\2\2\2\2Y\3\2\2\2\2]\3\2\2\2\2_\3\2\2\2\2a\3\2\2\2\3c\3\2\2\2\5g\3"+
		"\2\2\2\7m\3\2\2\2\tr\3\2\2\2\13x\3\2\2\2\r\177\3\2\2\2\17\u0082\3\2\2"+
		"\2\21\u0087\3\2\2\2\23\u008b\3\2\2\2\25\u0091\3\2\2\2\27\u0094\3\2\2\2"+
		"\31\u009a\3\2\2\2\33\u00a3\3\2\2\2\35\u00a5\3\2\2\2\37\u00a7\3\2\2\2!"+
		"\u00a9\3\2\2\2#\u00ab\3\2\2\2%\u00ad\3\2\2\2\'\u00af\3\2\2\2)\u00b1\3"+
		"\2\2\2+\u00b3\3\2\2\2-\u00b5\3\2\2\2/\u00b7\3\2\2\2\61\u00b9\3\2\2\2\63"+
		"\u00bb\3\2\2\2\65\u00bd\3\2\2\2\67\u00bf\3\2\2\29\u00c1\3\2\2\2;\u00c3"+
		"\3\2\2\2=\u00c5\3\2\2\2?\u00c8\3\2\2\2A\u00cb\3\2\2\2C\u00ce\3\2\2\2E"+
		"\u00d1\3\2\2\2G\u00d3\3\2\2\2I\u00d6\3\2\2\2K\u00d8\3\2\2\2M\u00f0\3\2"+
		"\2\2O\u0102\3\2\2\2Q\u0104\3\2\2\2S\u0129\3\2\2\2U\u0134\3\2\2\2W\u0136"+
		"\3\2\2\2Y\u013d\3\2\2\2[\u014b\3\2\2\2]\u014d\3\2\2\2_\u0151\3\2\2\2a"+
		"\u0161\3\2\2\2cd\7k\2\2de\7p\2\2ef\7v\2\2f\4\3\2\2\2gh\7h\2\2hi\7n\2\2"+
		"ij\7q\2\2jk\7c\2\2kl\7v\2\2l\6\3\2\2\2mn\7x\2\2no\7q\2\2op\7k\2\2pq\7"+
		"f\2\2q\b\3\2\2\2rs\7e\2\2st\7q\2\2tu\7p\2\2uv\7u\2\2vw\7v\2\2w\n\3\2\2"+
		"\2xy\7t\2\2yz\7g\2\2z{\7v\2\2{|\7w\2\2|}\7t\2\2}~\7p\2\2~\f\3\2\2\2\177"+
		"\u0080\7k\2\2\u0080\u0081\7h\2\2\u0081\16\3\2\2\2\u0082\u0083\7g\2\2\u0083"+
		"\u0084\7n\2\2\u0084\u0085\7u\2\2\u0085\u0086\7g\2\2\u0086\20\3\2\2\2\u0087"+
		"\u0088\7h\2\2\u0088\u0089\7q\2\2\u0089\u008a\7t\2\2\u008a\22\3\2\2\2\u008b"+
		"\u008c\7y\2\2\u008c\u008d\7j\2\2\u008d\u008e\7k\2\2\u008e\u008f\7n\2\2"+
		"\u008f\u0090\7g\2\2\u0090\24\3\2\2\2\u0091\u0092\7f\2\2\u0092\u0093\7"+
		"q\2\2\u0093\26\3\2\2\2\u0094\u0095\7d\2\2\u0095\u0096\7t\2\2\u0096\u0097"+
		"\7g\2\2\u0097\u0098\7c\2\2\u0098\u0099\7m\2\2\u0099\30\3\2\2\2\u009a\u009b"+
		"\7e\2\2\u009b\u009c\7q\2\2\u009c\u009d\7p\2\2\u009d\u009e\7v\2\2\u009e"+
		"\u009f\7k\2\2\u009f\u00a0\7p\2\2\u00a0\u00a1\7w\2\2\u00a1\u00a2\7g\2\2"+
		"\u00a2\32\3\2\2\2\u00a3\u00a4\7*\2\2\u00a4\34\3\2\2\2\u00a5\u00a6\7+\2"+
		"\2\u00a6\36\3\2\2\2\u00a7\u00a8\7]\2\2\u00a8 \3\2\2\2\u00a9\u00aa\7_\2"+
		"\2\u00aa\"\3\2\2\2\u00ab\u00ac\7}\2\2\u00ac$\3\2\2\2\u00ad\u00ae\7\177"+
		"\2\2\u00ae&\3\2\2\2\u00af\u00b0\7.\2\2\u00b0(\3\2\2\2\u00b1\u00b2\7=\2"+
		"\2\u00b2*\3\2\2\2\u00b3\u00b4\7A\2\2\u00b4,\3\2\2\2\u00b5\u00b6\7<\2\2"+
		"\u00b6.\3\2\2\2\u00b7\u00b8\7/\2\2\u00b8\60\3\2\2\2\u00b9\u00ba\7#\2\2"+
		"\u00ba\62\3\2\2\2\u00bb\u00bc\7\u0080\2\2\u00bc\64\3\2\2\2\u00bd\u00be"+
		"\7-\2\2\u00be\66\3\2\2\2\u00bf\u00c0\7,\2\2\u00c08\3\2\2\2\u00c1\u00c2"+
		"\7\61\2\2\u00c2:\3\2\2\2\u00c3\u00c4\7\'\2\2\u00c4<\3\2\2\2\u00c5\u00c6"+
		"\7(\2\2\u00c6\u00c7\7(\2\2\u00c7>\3\2\2\2\u00c8\u00c9\7~\2\2\u00c9\u00ca"+
		"\7~\2\2\u00ca@\3\2\2\2\u00cb\u00cc\7?\2\2\u00cc\u00cd\7?\2\2\u00cdB\3"+
		"\2\2\2\u00ce\u00cf\7#\2\2\u00cf\u00d0\7?\2\2\u00d0D\3\2\2\2\u00d1\u00d2"+
		"\7>\2\2\u00d2F\3\2\2\2\u00d3\u00d4\7>\2\2\u00d4\u00d5\7?\2\2\u00d5H\3"+
		"\2\2\2\u00d6\u00d7\7@\2\2\u00d7J\3\2\2\2\u00d8\u00d9\7@\2\2\u00d9\u00da"+
		"\7?\2\2\u00daL\3\2\2\2\u00db\u00dd\t\2\2\2\u00dc\u00db\3\2\2\2\u00dd\u00de"+
		"\3\2\2\2\u00de\u00dc\3\2\2\2\u00de\u00df\3\2\2\2\u00df\u00f1\3\2\2\2\u00e0"+
		"\u00e1\7\62\2\2\u00e1\u00e2\7z\2\2\u00e2\u00e4\3\2\2\2\u00e3\u00e5\t\3"+
		"\2\2\u00e4\u00e3\3\2\2\2\u00e5\u00e6\3\2\2\2\u00e6\u00e4\3\2\2\2\u00e6"+
		"\u00e7\3\2\2\2\u00e7\u00f1\3\2\2\2\u00e8\u00e9\7\62\2\2\u00e9\u00ea\7"+
		"Z\2\2\u00ea\u00ec\3\2\2\2\u00eb\u00ed\t\3\2\2\u00ec\u00eb\3\2\2\2\u00ed"+
		"\u00ee\3\2\2\2\u00ee\u00ec\3\2\2\2\u00ee\u00ef\3\2\2\2\u00ef\u00f1\3\2"+
		"\2\2\u00f0\u00dc\3\2\2\2\u00f0\u00e0\3\2\2\2\u00f0\u00e8\3\2\2\2\u00f1"+
		"N\3\2\2\2\u00f2\u00f3\7\62\2\2\u00f3\u00f4\7z\2\2\u00f4\u00f6\3\2\2\2"+
		"\u00f5\u00f7\t\3\2\2\u00f6\u00f5\3\2\2\2\u00f7\u00f8\3\2\2\2\u00f8\u00f6"+
		"\3\2\2\2\u00f8\u00f9\3\2\2\2\u00f9\u0103\3\2\2\2\u00fa\u00fb\7\62\2\2"+
		"\u00fb\u00fc\7Z\2\2\u00fc\u00fe\3\2\2\2\u00fd\u00ff\t\3\2\2\u00fe\u00fd"+
		"\3\2\2\2\u00ff\u0100\3\2\2\2\u0100\u00fe\3\2\2\2\u0100\u0101\3\2\2\2\u0101"+
		"\u0103\3\2\2\2\u0102\u00f2\3\2\2\2\u0102\u00fa\3\2\2\2\u0103P\3\2\2\2"+
		"\u0104\u0105\t\4\2\2\u0105\u0107\t\5\2\2\u0106\u0108\t\2\2\2\u0107\u0106"+
		"\3\2\2\2\u0108\u0109\3\2\2\2\u0109\u0107\3\2\2\2\u0109\u010a\3\2\2\2\u010a"+
		"R\3\2\2\2\u010b\u010d\t\2\2\2\u010c\u010b\3\2\2\2\u010d\u010e\3\2\2\2"+
		"\u010e\u010c\3\2\2\2\u010e\u010f\3\2\2\2\u010f\u0110\3\2\2\2\u0110\u0114"+
		"\7\60\2\2\u0111\u0113\t\2\2\2\u0112\u0111\3\2\2\2\u0113\u0116\3\2\2\2"+
		"\u0114\u0112\3\2\2\2\u0114\u0115\3\2\2\2\u0115\u0118\3\2\2\2\u0116\u0114"+
		"\3\2\2\2\u0117\u0119\5Q)\2\u0118\u0117\3\2\2\2\u0118\u0119\3\2\2\2\u0119"+
		"\u012a\3\2\2\2\u011a\u011c\7\60\2\2\u011b\u011d\t\2\2\2\u011c\u011b\3"+
		"\2\2\2\u011d\u011e\3\2\2\2\u011e\u011c\3\2\2\2\u011e\u011f\3\2\2\2\u011f"+
		"\u0121\3\2\2\2\u0120\u0122\5Q)\2\u0121\u0120\3\2\2\2\u0121\u0122\3\2\2"+
		"\2\u0122\u012a\3\2\2\2\u0123\u0125\t\2\2\2\u0124\u0123\3\2\2\2\u0125\u0126"+
		"\3\2\2\2\u0126\u0124\3\2\2\2\u0126\u0127\3\2\2\2\u0127\u0128\3\2\2\2\u0128"+
		"\u012a\5Q)\2\u0129\u010c\3\2\2\2\u0129\u011a\3\2\2\2\u0129\u0124\3\2\2"+
		"\2\u012aT\3\2\2\2\u012b\u0135\5S*\2\u012c\u012e\5O(\2\u012d\u012c\3\2"+
		"\2\2\u012d\u012e\3\2\2\2\u012e\u012f\3\2\2\2\u012f\u0130\7\60\2\2\u0130"+
		"\u0135\5O(\2\u0131\u0132\5O(\2\u0132\u0133\7\60\2\2\u0133\u0135\3\2\2"+
		"\2\u0134\u012b\3\2\2\2\u0134\u012d\3\2\2\2\u0134\u0131\3\2\2\2\u0135V"+
		"\3\2\2\2\u0136\u013a\t\6\2\2\u0137\u0139\t\7\2\2\u0138\u0137\3\2\2\2\u0139"+
		"\u013c\3\2\2\2\u013a\u0138\3\2\2\2\u013a\u013b\3\2\2\2\u013bX\3\2\2\2"+
		"\u013c\u013a\3\2\2\2\u013d\u0142\7$\2\2\u013e\u0141\5[.\2\u013f\u0141"+
		"\13\2\2\2\u0140\u013e\3\2\2\2\u0140\u013f\3\2\2\2\u0141\u0144\3\2\2\2"+
		"\u0142\u0143\3\2\2\2\u0142\u0140\3\2\2\2\u0143\u0145\3\2\2\2\u0144\u0142"+
		"\3\2\2\2\u0145\u0146\7$\2\2\u0146Z\3\2\2\2\u0147\u0148\7^\2\2\u0148\u014c"+
		"\7$\2\2\u0149\u014a\7^\2\2\u014a\u014c\7^\2\2\u014b\u0147\3\2\2\2\u014b"+
		"\u0149\3\2\2\2\u014c\\\3\2\2\2\u014d\u014e\t\b\2\2\u014e\u014f\3\2\2\2"+
		"\u014f\u0150\b/\2\2\u0150^\3\2\2\2\u0151\u0152\7\61\2\2\u0152\u0153\7"+
		"\61\2\2\u0153\u0157\3\2\2\2\u0154\u0156\13\2\2\2\u0155\u0154\3\2\2\2\u0156"+
		"\u0159\3\2\2\2\u0157\u0158\3\2\2\2\u0157\u0155\3\2\2\2\u0158\u015b\3\2"+
		"\2\2\u0159\u0157\3\2\2\2\u015a\u015c\7\17\2\2\u015b\u015a\3\2\2\2\u015b"+
		"\u015c\3\2\2\2\u015c\u015d\3\2\2\2\u015d\u015e\7\f\2\2\u015e\u015f\3\2"+
		"\2\2\u015f\u0160\b\60\2\2\u0160`\3\2\2\2\u0161\u0162\7\61\2\2\u0162\u0163"+
		"\7,\2\2\u0163\u0167\3\2\2\2\u0164\u0166\13\2\2\2\u0165\u0164\3\2\2\2\u0166"+
		"\u0169\3\2\2\2\u0167\u0168\3\2\2\2\u0167\u0165\3\2\2\2\u0168\u016a\3\2"+
		"\2\2\u0169\u0167\3\2\2\2\u016a\u016b\7,\2\2\u016b\u016c\7\61\2\2\u016c"+
		"\u016d\3\2\2\2\u016d\u016e\b\61\2\2\u016eb\3\2\2\2\33\2\u00de\u00e6\u00ee"+
		"\u00f0\u00f8\u0100\u0102\u0109\u010e\u0114\u0118\u011e\u0121\u0126\u0129"+
		"\u012d\u0134\u013a\u0140\u0142\u014b\u0157\u015b\u0167\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}