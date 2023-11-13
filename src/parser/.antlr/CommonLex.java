// Generated from /mnt/f/test/src/parser/CommonLex.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class CommonLex extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Int=1, Float=2, Void=3, Const=4, Return=5, If=6, Else=7, For=8, While=9, 
		Do=10, Break=11, Continue=12, Lparen=13, Rparen=14, Lbrkt=15, Rbrkt=16, 
		Lbrace=17, Rbrace=18, Comma=19, Semicolon=20, Question=21, Colon=22, Minus=23, 
		Exclamation=24, Tilde=25, Addition=26, Multiplication=27, Division=28, 
		Modulo=29, LAND=30, LOR=31, EQ=32, NEQ=33, LT=34, LE=35, GT=36, GE=37, 
		Identifier=38, IntLiteral=39, HexadecimalDigits=40, ExponentPart=41, BinaryExponentPart=42, 
		FloatLiteral=43, STRING=44, WS=45, LINE_COMMENT=46, COMMENT=47;
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
			"LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", "Identifier", "IntLiteral", 
			"HexadecimalDigits", "ExponentPart", "BinaryExponentPart", "HexadecimalFloatingConstant", 
			"HexDigits", "HexDigit", "DecimalFloatingConstant", "FloatLiteral", "STRING", 
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
			"LAND", "LOR", "EQ", "NEQ", "LT", "LE", "GT", "GE", "Identifier", "IntLiteral", 
			"HexadecimalDigits", "ExponentPart", "BinaryExponentPart", "FloatLiteral", 
			"STRING", "WS", "LINE_COMMENT", "COMMENT"
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
		"\u0004\u0000/\u0190\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002\u0001"+
		"\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004"+
		"\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007"+
		"\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b"+
		"\u0007\u000b\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002"+
		"\u000f\u0007\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002"+
		"\u0012\u0007\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002"+
		"\u0015\u0007\u0015\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002"+
		"\u0018\u0007\u0018\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002"+
		"\u001b\u0007\u001b\u0002\u001c\u0007\u001c\u0002\u001d\u0007\u001d\u0002"+
		"\u001e\u0007\u001e\u0002\u001f\u0007\u001f\u0002 \u0007 \u0002!\u0007"+
		"!\u0002\"\u0007\"\u0002#\u0007#\u0002$\u0007$\u0002%\u0007%\u0002&\u0007"+
		"&\u0002\'\u0007\'\u0002(\u0007(\u0002)\u0007)\u0002*\u0007*\u0002+\u0007"+
		"+\u0002,\u0007,\u0002-\u0007-\u0002.\u0007.\u0002/\u0007/\u00020\u0007"+
		"0\u00021\u00071\u00022\u00072\u00023\u00073\u0001\u0000\u0001\u0000\u0001"+
		"\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001"+
		"\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0006\u0001"+
		"\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001"+
		"\u0007\u0001\u0007\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001"+
		"\t\u0001\t\u0001\t\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\f\u0001\f\u0001\r\u0001\r\u0001\u000e"+
		"\u0001\u000e\u0001\u000f\u0001\u000f\u0001\u0010\u0001\u0010\u0001\u0011"+
		"\u0001\u0011\u0001\u0012\u0001\u0012\u0001\u0013\u0001\u0013\u0001\u0014"+
		"\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0016\u0001\u0016\u0001\u0017"+
		"\u0001\u0017\u0001\u0018\u0001\u0018\u0001\u0019\u0001\u0019\u0001\u001a"+
		"\u0001\u001a\u0001\u001b\u0001\u001b\u0001\u001c\u0001\u001c\u0001\u001d"+
		"\u0001\u001d\u0001\u001d\u0001\u001e\u0001\u001e\u0001\u001e\u0001\u001f"+
		"\u0001\u001f\u0001\u001f\u0001 \u0001 \u0001 \u0001!\u0001!\u0001\"\u0001"+
		"\"\u0001\"\u0001#\u0001#\u0001$\u0001$\u0001$\u0001%\u0001%\u0005%\u00e4"+
		"\b%\n%\f%\u00e7\t%\u0001&\u0004&\u00ea\b&\u000b&\f&\u00eb\u0001&\u0001"+
		"&\u0001&\u0001&\u0004&\u00f2\b&\u000b&\f&\u00f3\u0001&\u0001&\u0001&\u0001"+
		"&\u0004&\u00fa\b&\u000b&\f&\u00fb\u0003&\u00fe\b&\u0001\'\u0001\'\u0001"+
		"\'\u0001\'\u0004\'\u0104\b\'\u000b\'\f\'\u0105\u0001\'\u0001\'\u0001\'"+
		"\u0001\'\u0004\'\u010c\b\'\u000b\'\f\'\u010d\u0003\'\u0110\b\'\u0001("+
		"\u0001(\u0003(\u0114\b(\u0001(\u0004(\u0117\b(\u000b(\f(\u0118\u0001)"+
		"\u0001)\u0003)\u011d\b)\u0001)\u0004)\u0120\b)\u000b)\f)\u0121\u0001*"+
		"\u0001*\u0001*\u0003*\u0127\b*\u0001*\u0001*\u0001*\u0001*\u0001*\u0001"+
		"*\u0001*\u0001*\u0001*\u0003*\u0132\b*\u0001+\u0004+\u0135\b+\u000b+\f"+
		"+\u0136\u0001,\u0001,\u0001-\u0004-\u013c\b-\u000b-\f-\u013d\u0001-\u0001"+
		"-\u0005-\u0142\b-\n-\f-\u0145\t-\u0001-\u0003-\u0148\b-\u0001-\u0001-"+
		"\u0004-\u014c\b-\u000b-\f-\u014d\u0001-\u0003-\u0151\b-\u0001-\u0004-"+
		"\u0154\b-\u000b-\f-\u0155\u0001-\u0003-\u0159\b-\u0001.\u0001.\u0003."+
		"\u015d\b.\u0001/\u0001/\u0001/\u0005/\u0162\b/\n/\f/\u0165\t/\u0001/\u0001"+
		"/\u00010\u00010\u00010\u00010\u00030\u016d\b0\u00011\u00011\u00011\u0001"+
		"1\u00012\u00012\u00012\u00012\u00052\u0177\b2\n2\f2\u017a\t2\u00012\u0003"+
		"2\u017d\b2\u00012\u00012\u00012\u00012\u00013\u00013\u00013\u00013\u0005"+
		"3\u0187\b3\n3\f3\u018a\t3\u00013\u00013\u00013\u00013\u00013\u0003\u0163"+
		"\u0178\u0188\u00004\u0001\u0001\u0003\u0002\u0005\u0003\u0007\u0004\t"+
		"\u0005\u000b\u0006\r\u0007\u000f\b\u0011\t\u0013\n\u0015\u000b\u0017\f"+
		"\u0019\r\u001b\u000e\u001d\u000f\u001f\u0010!\u0011#\u0012%\u0013\'\u0014"+
		")\u0015+\u0016-\u0017/\u00181\u00193\u001a5\u001b7\u001c9\u001d;\u001e"+
		"=\u001f? A!C\"E#G$I%K&M\'O(Q)S*U\u0000W\u0000Y\u0000[\u0000]+_,a\u0000"+
		"c-e.g/\u0001\u0000\t\u0003\u0000AZ__az\u0004\u000009AZ__az\u0001\u0000"+
		"09\u0003\u000009AFaf\u0002\u0000EEee\u0002\u0000++--\u0002\u0000PPpp\u0002"+
		"\u0000XXxx\u0003\u0000\t\n\r\r  \u01a9\u0000\u0001\u0001\u0000\u0000\u0000"+
		"\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u0005\u0001\u0000\u0000\u0000"+
		"\u0000\u0007\u0001\u0000\u0000\u0000\u0000\t\u0001\u0000\u0000\u0000\u0000"+
		"\u000b\u0001\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000\u0000\u000f"+
		"\u0001\u0000\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000\u0000\u0013"+
		"\u0001\u0000\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000\u0000\u0017"+
		"\u0001\u0000\u0000\u0000\u0000\u0019\u0001\u0000\u0000\u0000\u0000\u001b"+
		"\u0001\u0000\u0000\u0000\u0000\u001d\u0001\u0000\u0000\u0000\u0000\u001f"+
		"\u0001\u0000\u0000\u0000\u0000!\u0001\u0000\u0000\u0000\u0000#\u0001\u0000"+
		"\u0000\u0000\u0000%\u0001\u0000\u0000\u0000\u0000\'\u0001\u0000\u0000"+
		"\u0000\u0000)\u0001\u0000\u0000\u0000\u0000+\u0001\u0000\u0000\u0000\u0000"+
		"-\u0001\u0000\u0000\u0000\u0000/\u0001\u0000\u0000\u0000\u00001\u0001"+
		"\u0000\u0000\u0000\u00003\u0001\u0000\u0000\u0000\u00005\u0001\u0000\u0000"+
		"\u0000\u00007\u0001\u0000\u0000\u0000\u00009\u0001\u0000\u0000\u0000\u0000"+
		";\u0001\u0000\u0000\u0000\u0000=\u0001\u0000\u0000\u0000\u0000?\u0001"+
		"\u0000\u0000\u0000\u0000A\u0001\u0000\u0000\u0000\u0000C\u0001\u0000\u0000"+
		"\u0000\u0000E\u0001\u0000\u0000\u0000\u0000G\u0001\u0000\u0000\u0000\u0000"+
		"I\u0001\u0000\u0000\u0000\u0000K\u0001\u0000\u0000\u0000\u0000M\u0001"+
		"\u0000\u0000\u0000\u0000O\u0001\u0000\u0000\u0000\u0000Q\u0001\u0000\u0000"+
		"\u0000\u0000S\u0001\u0000\u0000\u0000\u0000]\u0001\u0000\u0000\u0000\u0000"+
		"_\u0001\u0000\u0000\u0000\u0000c\u0001\u0000\u0000\u0000\u0000e\u0001"+
		"\u0000\u0000\u0000\u0000g\u0001\u0000\u0000\u0000\u0001i\u0001\u0000\u0000"+
		"\u0000\u0003m\u0001\u0000\u0000\u0000\u0005s\u0001\u0000\u0000\u0000\u0007"+
		"x\u0001\u0000\u0000\u0000\t~\u0001\u0000\u0000\u0000\u000b\u0085\u0001"+
		"\u0000\u0000\u0000\r\u0088\u0001\u0000\u0000\u0000\u000f\u008d\u0001\u0000"+
		"\u0000\u0000\u0011\u0091\u0001\u0000\u0000\u0000\u0013\u0097\u0001\u0000"+
		"\u0000\u0000\u0015\u009a\u0001\u0000\u0000\u0000\u0017\u00a0\u0001\u0000"+
		"\u0000\u0000\u0019\u00a9\u0001\u0000\u0000\u0000\u001b\u00ab\u0001\u0000"+
		"\u0000\u0000\u001d\u00ad\u0001\u0000\u0000\u0000\u001f\u00af\u0001\u0000"+
		"\u0000\u0000!\u00b1\u0001\u0000\u0000\u0000#\u00b3\u0001\u0000\u0000\u0000"+
		"%\u00b5\u0001\u0000\u0000\u0000\'\u00b7\u0001\u0000\u0000\u0000)\u00b9"+
		"\u0001\u0000\u0000\u0000+\u00bb\u0001\u0000\u0000\u0000-\u00bd\u0001\u0000"+
		"\u0000\u0000/\u00bf\u0001\u0000\u0000\u00001\u00c1\u0001\u0000\u0000\u0000"+
		"3\u00c3\u0001\u0000\u0000\u00005\u00c5\u0001\u0000\u0000\u00007\u00c7"+
		"\u0001\u0000\u0000\u00009\u00c9\u0001\u0000\u0000\u0000;\u00cb\u0001\u0000"+
		"\u0000\u0000=\u00ce\u0001\u0000\u0000\u0000?\u00d1\u0001\u0000\u0000\u0000"+
		"A\u00d4\u0001\u0000\u0000\u0000C\u00d7\u0001\u0000\u0000\u0000E\u00d9"+
		"\u0001\u0000\u0000\u0000G\u00dc\u0001\u0000\u0000\u0000I\u00de\u0001\u0000"+
		"\u0000\u0000K\u00e1\u0001\u0000\u0000\u0000M\u00fd\u0001\u0000\u0000\u0000"+
		"O\u010f\u0001\u0000\u0000\u0000Q\u0111\u0001\u0000\u0000\u0000S\u011a"+
		"\u0001\u0000\u0000\u0000U\u0131\u0001\u0000\u0000\u0000W\u0134\u0001\u0000"+
		"\u0000\u0000Y\u0138\u0001\u0000\u0000\u0000[\u0158\u0001\u0000\u0000\u0000"+
		"]\u015c\u0001\u0000\u0000\u0000_\u015e\u0001\u0000\u0000\u0000a\u016c"+
		"\u0001\u0000\u0000\u0000c\u016e\u0001\u0000\u0000\u0000e\u0172\u0001\u0000"+
		"\u0000\u0000g\u0182\u0001\u0000\u0000\u0000ij\u0005i\u0000\u0000jk\u0005"+
		"n\u0000\u0000kl\u0005t\u0000\u0000l\u0002\u0001\u0000\u0000\u0000mn\u0005"+
		"f\u0000\u0000no\u0005l\u0000\u0000op\u0005o\u0000\u0000pq\u0005a\u0000"+
		"\u0000qr\u0005t\u0000\u0000r\u0004\u0001\u0000\u0000\u0000st\u0005v\u0000"+
		"\u0000tu\u0005o\u0000\u0000uv\u0005i\u0000\u0000vw\u0005d\u0000\u0000"+
		"w\u0006\u0001\u0000\u0000\u0000xy\u0005c\u0000\u0000yz\u0005o\u0000\u0000"+
		"z{\u0005n\u0000\u0000{|\u0005s\u0000\u0000|}\u0005t\u0000\u0000}\b\u0001"+
		"\u0000\u0000\u0000~\u007f\u0005r\u0000\u0000\u007f\u0080\u0005e\u0000"+
		"\u0000\u0080\u0081\u0005t\u0000\u0000\u0081\u0082\u0005u\u0000\u0000\u0082"+
		"\u0083\u0005r\u0000\u0000\u0083\u0084\u0005n\u0000\u0000\u0084\n\u0001"+
		"\u0000\u0000\u0000\u0085\u0086\u0005i\u0000\u0000\u0086\u0087\u0005f\u0000"+
		"\u0000\u0087\f\u0001\u0000\u0000\u0000\u0088\u0089\u0005e\u0000\u0000"+
		"\u0089\u008a\u0005l\u0000\u0000\u008a\u008b\u0005s\u0000\u0000\u008b\u008c"+
		"\u0005e\u0000\u0000\u008c\u000e\u0001\u0000\u0000\u0000\u008d\u008e\u0005"+
		"f\u0000\u0000\u008e\u008f\u0005o\u0000\u0000\u008f\u0090\u0005r\u0000"+
		"\u0000\u0090\u0010\u0001\u0000\u0000\u0000\u0091\u0092\u0005w\u0000\u0000"+
		"\u0092\u0093\u0005h\u0000\u0000\u0093\u0094\u0005i\u0000\u0000\u0094\u0095"+
		"\u0005l\u0000\u0000\u0095\u0096\u0005e\u0000\u0000\u0096\u0012\u0001\u0000"+
		"\u0000\u0000\u0097\u0098\u0005d\u0000\u0000\u0098\u0099\u0005o\u0000\u0000"+
		"\u0099\u0014\u0001\u0000\u0000\u0000\u009a\u009b\u0005b\u0000\u0000\u009b"+
		"\u009c\u0005r\u0000\u0000\u009c\u009d\u0005e\u0000\u0000\u009d\u009e\u0005"+
		"a\u0000\u0000\u009e\u009f\u0005k\u0000\u0000\u009f\u0016\u0001\u0000\u0000"+
		"\u0000\u00a0\u00a1\u0005c\u0000\u0000\u00a1\u00a2\u0005o\u0000\u0000\u00a2"+
		"\u00a3\u0005n\u0000\u0000\u00a3\u00a4\u0005t\u0000\u0000\u00a4\u00a5\u0005"+
		"i\u0000\u0000\u00a5\u00a6\u0005n\u0000\u0000\u00a6\u00a7\u0005u\u0000"+
		"\u0000\u00a7\u00a8\u0005e\u0000\u0000\u00a8\u0018\u0001\u0000\u0000\u0000"+
		"\u00a9\u00aa\u0005(\u0000\u0000\u00aa\u001a\u0001\u0000\u0000\u0000\u00ab"+
		"\u00ac\u0005)\u0000\u0000\u00ac\u001c\u0001\u0000\u0000\u0000\u00ad\u00ae"+
		"\u0005[\u0000\u0000\u00ae\u001e\u0001\u0000\u0000\u0000\u00af\u00b0\u0005"+
		"]\u0000\u0000\u00b0 \u0001\u0000\u0000\u0000\u00b1\u00b2\u0005{\u0000"+
		"\u0000\u00b2\"\u0001\u0000\u0000\u0000\u00b3\u00b4\u0005}\u0000\u0000"+
		"\u00b4$\u0001\u0000\u0000\u0000\u00b5\u00b6\u0005,\u0000\u0000\u00b6&"+
		"\u0001\u0000\u0000\u0000\u00b7\u00b8\u0005;\u0000\u0000\u00b8(\u0001\u0000"+
		"\u0000\u0000\u00b9\u00ba\u0005?\u0000\u0000\u00ba*\u0001\u0000\u0000\u0000"+
		"\u00bb\u00bc\u0005:\u0000\u0000\u00bc,\u0001\u0000\u0000\u0000\u00bd\u00be"+
		"\u0005-\u0000\u0000\u00be.\u0001\u0000\u0000\u0000\u00bf\u00c0\u0005!"+
		"\u0000\u0000\u00c00\u0001\u0000\u0000\u0000\u00c1\u00c2\u0005~\u0000\u0000"+
		"\u00c22\u0001\u0000\u0000\u0000\u00c3\u00c4\u0005+\u0000\u0000\u00c44"+
		"\u0001\u0000\u0000\u0000\u00c5\u00c6\u0005*\u0000\u0000\u00c66\u0001\u0000"+
		"\u0000\u0000\u00c7\u00c8\u0005/\u0000\u0000\u00c88\u0001\u0000\u0000\u0000"+
		"\u00c9\u00ca\u0005%\u0000\u0000\u00ca:\u0001\u0000\u0000\u0000\u00cb\u00cc"+
		"\u0005&\u0000\u0000\u00cc\u00cd\u0005&\u0000\u0000\u00cd<\u0001\u0000"+
		"\u0000\u0000\u00ce\u00cf\u0005|\u0000\u0000\u00cf\u00d0\u0005|\u0000\u0000"+
		"\u00d0>\u0001\u0000\u0000\u0000\u00d1\u00d2\u0005=\u0000\u0000\u00d2\u00d3"+
		"\u0005=\u0000\u0000\u00d3@\u0001\u0000\u0000\u0000\u00d4\u00d5\u0005!"+
		"\u0000\u0000\u00d5\u00d6\u0005=\u0000\u0000\u00d6B\u0001\u0000\u0000\u0000"+
		"\u00d7\u00d8\u0005<\u0000\u0000\u00d8D\u0001\u0000\u0000\u0000\u00d9\u00da"+
		"\u0005<\u0000\u0000\u00da\u00db\u0005=\u0000\u0000\u00dbF\u0001\u0000"+
		"\u0000\u0000\u00dc\u00dd\u0005>\u0000\u0000\u00ddH\u0001\u0000\u0000\u0000"+
		"\u00de\u00df\u0005>\u0000\u0000\u00df\u00e0\u0005=\u0000\u0000\u00e0J"+
		"\u0001\u0000\u0000\u0000\u00e1\u00e5\u0007\u0000\u0000\u0000\u00e2\u00e4"+
		"\u0007\u0001\u0000\u0000\u00e3\u00e2\u0001\u0000\u0000\u0000\u00e4\u00e7"+
		"\u0001\u0000\u0000\u0000\u00e5\u00e3\u0001\u0000\u0000\u0000\u00e5\u00e6"+
		"\u0001\u0000\u0000\u0000\u00e6L\u0001\u0000\u0000\u0000\u00e7\u00e5\u0001"+
		"\u0000\u0000\u0000\u00e8\u00ea\u0007\u0002\u0000\u0000\u00e9\u00e8\u0001"+
		"\u0000\u0000\u0000\u00ea\u00eb\u0001\u0000\u0000\u0000\u00eb\u00e9\u0001"+
		"\u0000\u0000\u0000\u00eb\u00ec\u0001\u0000\u0000\u0000\u00ec\u00fe\u0001"+
		"\u0000\u0000\u0000\u00ed\u00ee\u00050\u0000\u0000\u00ee\u00ef\u0005x\u0000"+
		"\u0000\u00ef\u00f1\u0001\u0000\u0000\u0000\u00f0\u00f2\u0007\u0003\u0000"+
		"\u0000\u00f1\u00f0\u0001\u0000\u0000\u0000\u00f2\u00f3\u0001\u0000\u0000"+
		"\u0000\u00f3\u00f1\u0001\u0000\u0000\u0000\u00f3\u00f4\u0001\u0000\u0000"+
		"\u0000\u00f4\u00fe\u0001\u0000\u0000\u0000\u00f5\u00f6\u00050\u0000\u0000"+
		"\u00f6\u00f7\u0005X\u0000\u0000\u00f7\u00f9\u0001\u0000\u0000\u0000\u00f8"+
		"\u00fa\u0007\u0003\u0000\u0000\u00f9\u00f8\u0001\u0000\u0000\u0000\u00fa"+
		"\u00fb\u0001\u0000\u0000\u0000\u00fb\u00f9\u0001\u0000\u0000\u0000\u00fb"+
		"\u00fc\u0001\u0000\u0000\u0000\u00fc\u00fe\u0001\u0000\u0000\u0000\u00fd"+
		"\u00e9\u0001\u0000\u0000\u0000\u00fd\u00ed\u0001\u0000\u0000\u0000\u00fd"+
		"\u00f5\u0001\u0000\u0000\u0000\u00feN\u0001\u0000\u0000\u0000\u00ff\u0100"+
		"\u00050\u0000\u0000\u0100\u0101\u0005x\u0000\u0000\u0101\u0103\u0001\u0000"+
		"\u0000\u0000\u0102\u0104\u0007\u0003\u0000\u0000\u0103\u0102\u0001\u0000"+
		"\u0000\u0000\u0104\u0105\u0001\u0000\u0000\u0000\u0105\u0103\u0001\u0000"+
		"\u0000\u0000\u0105\u0106\u0001\u0000\u0000\u0000\u0106\u0110\u0001\u0000"+
		"\u0000\u0000\u0107\u0108\u00050\u0000\u0000\u0108\u0109\u0005X\u0000\u0000"+
		"\u0109\u010b\u0001\u0000\u0000\u0000\u010a\u010c\u0007\u0003\u0000\u0000"+
		"\u010b\u010a\u0001\u0000\u0000\u0000\u010c\u010d\u0001\u0000\u0000\u0000"+
		"\u010d\u010b\u0001\u0000\u0000\u0000\u010d\u010e\u0001\u0000\u0000\u0000"+
		"\u010e\u0110\u0001\u0000\u0000\u0000\u010f\u00ff\u0001\u0000\u0000\u0000"+
		"\u010f\u0107\u0001\u0000\u0000\u0000\u0110P\u0001\u0000\u0000\u0000\u0111"+
		"\u0113\u0007\u0004\u0000\u0000\u0112\u0114\u0007\u0005\u0000\u0000\u0113"+
		"\u0112\u0001\u0000\u0000\u0000\u0113\u0114\u0001\u0000\u0000\u0000\u0114"+
		"\u0116\u0001\u0000\u0000\u0000\u0115\u0117\u0007\u0002\u0000\u0000\u0116"+
		"\u0115\u0001\u0000\u0000\u0000\u0117\u0118\u0001\u0000\u0000\u0000\u0118"+
		"\u0116\u0001\u0000\u0000\u0000\u0118\u0119\u0001\u0000\u0000\u0000\u0119"+
		"R\u0001\u0000\u0000\u0000\u011a\u011c\u0007\u0006\u0000\u0000\u011b\u011d"+
		"\u0007\u0005\u0000\u0000\u011c\u011b\u0001\u0000\u0000\u0000\u011c\u011d"+
		"\u0001\u0000\u0000\u0000\u011d\u011f\u0001\u0000\u0000\u0000\u011e\u0120"+
		"\u0007\u0002\u0000\u0000\u011f\u011e\u0001\u0000\u0000\u0000\u0120\u0121"+
		"\u0001\u0000\u0000\u0000\u0121\u011f\u0001\u0000\u0000\u0000\u0121\u0122"+
		"\u0001\u0000\u0000\u0000\u0122T\u0001\u0000\u0000\u0000\u0123\u0124\u0005"+
		"0\u0000\u0000\u0124\u0126\u0007\u0007\u0000\u0000\u0125\u0127\u0003W+"+
		"\u0000\u0126\u0125\u0001\u0000\u0000\u0000\u0126\u0127\u0001\u0000\u0000"+
		"\u0000\u0127\u0128\u0001\u0000\u0000\u0000\u0128\u0129\u0005.\u0000\u0000"+
		"\u0129\u012a\u0003W+\u0000\u012a\u012b\u0003S)\u0000\u012b\u0132\u0001"+
		"\u0000\u0000\u0000\u012c\u012d\u00050\u0000\u0000\u012d\u012e\u0007\u0007"+
		"\u0000\u0000\u012e\u012f\u0003W+\u0000\u012f\u0130\u0003S)\u0000\u0130"+
		"\u0132\u0001\u0000\u0000\u0000\u0131\u0123\u0001\u0000\u0000\u0000\u0131"+
		"\u012c\u0001\u0000\u0000\u0000\u0132V\u0001\u0000\u0000\u0000\u0133\u0135"+
		"\u0003Y,\u0000\u0134\u0133\u0001\u0000\u0000\u0000\u0135\u0136\u0001\u0000"+
		"\u0000\u0000\u0136\u0134\u0001\u0000\u0000\u0000\u0136\u0137\u0001\u0000"+
		"\u0000\u0000\u0137X\u0001\u0000\u0000\u0000\u0138\u0139\u0007\u0003\u0000"+
		"\u0000\u0139Z\u0001\u0000\u0000\u0000\u013a\u013c\u0007\u0002\u0000\u0000"+
		"\u013b\u013a\u0001\u0000\u0000\u0000\u013c\u013d\u0001\u0000\u0000\u0000"+
		"\u013d\u013b\u0001\u0000\u0000\u0000\u013d\u013e\u0001\u0000\u0000\u0000"+
		"\u013e\u013f\u0001\u0000\u0000\u0000\u013f\u0143\u0005.\u0000\u0000\u0140"+
		"\u0142\u0007\u0002\u0000\u0000\u0141\u0140\u0001\u0000\u0000\u0000\u0142"+
		"\u0145\u0001\u0000\u0000\u0000\u0143\u0141\u0001\u0000\u0000\u0000\u0143"+
		"\u0144\u0001\u0000\u0000\u0000\u0144\u0147\u0001\u0000\u0000\u0000\u0145"+
		"\u0143\u0001\u0000\u0000\u0000\u0146\u0148\u0003Q(\u0000\u0147\u0146\u0001"+
		"\u0000\u0000\u0000\u0147\u0148\u0001\u0000\u0000\u0000\u0148\u0159\u0001"+
		"\u0000\u0000\u0000\u0149\u014b\u0005.\u0000\u0000\u014a\u014c\u0007\u0002"+
		"\u0000\u0000\u014b\u014a\u0001\u0000\u0000\u0000\u014c\u014d\u0001\u0000"+
		"\u0000\u0000\u014d\u014b\u0001\u0000\u0000\u0000\u014d\u014e\u0001\u0000"+
		"\u0000\u0000\u014e\u0150\u0001\u0000\u0000\u0000\u014f\u0151\u0003Q(\u0000"+
		"\u0150\u014f\u0001\u0000\u0000\u0000\u0150\u0151\u0001\u0000\u0000\u0000"+
		"\u0151\u0159\u0001\u0000\u0000\u0000\u0152\u0154\u0007\u0002\u0000\u0000"+
		"\u0153\u0152\u0001\u0000\u0000\u0000\u0154\u0155\u0001\u0000\u0000\u0000"+
		"\u0155\u0153\u0001\u0000\u0000\u0000\u0155\u0156\u0001\u0000\u0000\u0000"+
		"\u0156\u0157\u0001\u0000\u0000\u0000\u0157\u0159\u0003Q(\u0000\u0158\u013b"+
		"\u0001\u0000\u0000\u0000\u0158\u0149\u0001\u0000\u0000\u0000\u0158\u0153"+
		"\u0001\u0000\u0000\u0000\u0159\\\u0001\u0000\u0000\u0000\u015a\u015d\u0003"+
		"[-\u0000\u015b\u015d\u0003U*\u0000\u015c\u015a\u0001\u0000\u0000\u0000"+
		"\u015c\u015b\u0001\u0000\u0000\u0000\u015d^\u0001\u0000\u0000\u0000\u015e"+
		"\u0163\u0005\"\u0000\u0000\u015f\u0162\u0003a0\u0000\u0160\u0162\t\u0000"+
		"\u0000\u0000\u0161\u015f\u0001\u0000\u0000\u0000\u0161\u0160\u0001\u0000"+
		"\u0000\u0000\u0162\u0165\u0001\u0000\u0000\u0000\u0163\u0164\u0001\u0000"+
		"\u0000\u0000\u0163\u0161\u0001\u0000\u0000\u0000\u0164\u0166\u0001\u0000"+
		"\u0000\u0000\u0165\u0163\u0001\u0000\u0000\u0000\u0166\u0167\u0005\"\u0000"+
		"\u0000\u0167`\u0001\u0000\u0000\u0000\u0168\u0169\u0005\\\u0000\u0000"+
		"\u0169\u016d\u0005\"\u0000\u0000\u016a\u016b\u0005\\\u0000\u0000\u016b"+
		"\u016d\u0005\\\u0000\u0000\u016c\u0168\u0001\u0000\u0000\u0000\u016c\u016a"+
		"\u0001\u0000\u0000\u0000\u016db\u0001\u0000\u0000\u0000\u016e\u016f\u0007"+
		"\b\u0000\u0000\u016f\u0170\u0001\u0000\u0000\u0000\u0170\u0171\u00061"+
		"\u0000\u0000\u0171d\u0001\u0000\u0000\u0000\u0172\u0173\u0005/\u0000\u0000"+
		"\u0173\u0174\u0005/\u0000\u0000\u0174\u0178\u0001\u0000\u0000\u0000\u0175"+
		"\u0177\t\u0000\u0000\u0000\u0176\u0175\u0001\u0000\u0000\u0000\u0177\u017a"+
		"\u0001\u0000\u0000\u0000\u0178\u0179\u0001\u0000\u0000\u0000\u0178\u0176"+
		"\u0001\u0000\u0000\u0000\u0179\u017c\u0001\u0000\u0000\u0000\u017a\u0178"+
		"\u0001\u0000\u0000\u0000\u017b\u017d\u0005\r\u0000\u0000\u017c\u017b\u0001"+
		"\u0000\u0000\u0000\u017c\u017d\u0001\u0000\u0000\u0000\u017d\u017e\u0001"+
		"\u0000\u0000\u0000\u017e\u017f\u0005\n\u0000\u0000\u017f\u0180\u0001\u0000"+
		"\u0000\u0000\u0180\u0181\u00062\u0000\u0000\u0181f\u0001\u0000\u0000\u0000"+
		"\u0182\u0183\u0005/\u0000\u0000\u0183\u0184\u0005*\u0000\u0000\u0184\u0188"+
		"\u0001\u0000\u0000\u0000\u0185\u0187\t\u0000\u0000\u0000\u0186\u0185\u0001"+
		"\u0000\u0000\u0000\u0187\u018a\u0001\u0000\u0000\u0000\u0188\u0189\u0001"+
		"\u0000\u0000\u0000\u0188\u0186\u0001\u0000\u0000\u0000\u0189\u018b\u0001"+
		"\u0000\u0000\u0000\u018a\u0188\u0001\u0000\u0000\u0000\u018b\u018c\u0005"+
		"*\u0000\u0000\u018c\u018d\u0005/\u0000\u0000\u018d\u018e\u0001\u0000\u0000"+
		"\u0000\u018e\u018f\u00063\u0000\u0000\u018fh\u0001\u0000\u0000\u0000\u001e"+
		"\u0000\u00e5\u00eb\u00f3\u00fb\u00fd\u0105\u010d\u010f\u0113\u0118\u011c"+
		"\u0121\u0126\u0131\u0136\u013d\u0143\u0147\u014d\u0150\u0155\u0158\u015c"+
		"\u0161\u0163\u016c\u0178\u017c\u0188\u0001\u0006\u0000\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}