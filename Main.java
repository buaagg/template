import java.io.*;
import java.util.*;
import java.math.*;

class Task {
	void solve( int ri, InputReader in, PrintWriter out ) {
		BigDecimal a = new BigDecimal("23213432.2142143");
		a = a.round( new MathContext(10, RoundingMode.HALF_UP) );
		out.println( a.toPlainString() );
	}
}

public class Main {
	public static void main(String []args) {
		InputStream insm = System.in;
		OutputStream outsm = System.out;
		InputReader in = new InputReader( insm );
		PrintWriter out = new PrintWriter( outsm );
		Task task = new Task();
		task.solve(1, in, out);
		out.close();
	}
}

class InputReader {
	private BufferedReader reader;
	private StringTokenizer tokenizer;

	public InputReader( InputStream sm ) {
		reader = new BufferedReader( new InputStreamReader(sm) );
		tokenizer = null;
	}

	public String next() {
		while ( tokenizer == null || !tokenizer.hasMoreTokens() ) {
			try {
				tokenizer = new StringTokenizer( reader.readLine() );
			} catch( IOException e ) {
				throw new RuntimeException(e);
			}
		}
		return tokenizer.nextToken();
	}
}
