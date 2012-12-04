using System;
using Mono.CSharp;
 
class StorageCompiler {
   public Evaluator Evaluator;

   public StorageCompiler (string [] args)
   {
		   var r = new Report (new ConsoleReportPrinter ());
		   var cmd = new CommandLineParser (r);

		   var settings = cmd.ParseArguments (args);
		   if (settings == null || r.Errors > 0)
				   Environment.Exit (1);

		   Evaluator = new Evaluator (settings, r);
   }
}
 
class X {
   static StorageCompiler sc = null;	
   static int Main (string [] args)
   {
		   sc = new StorageCompiler (args);
		   return 0;
   }
   
   
}