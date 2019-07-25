using NUnit.Framework;
using System;
using System.IO;

namespace VCDiffSharp.UntiTests {
	[TestFixture ()]
	public class Test {
		[Test ()]
		public void TestCase ()
		{
			File.WriteAllText ("input.txt", "1234");
			File.WriteAllText ("target.txt", "4321");
			File.Delete ("patch.txt");

			Assert.AreEqual (true, VCDiffSharp.VCDiff.Encode ("input.txt", "target.txt", "patch.txt"));
			Assert.AreEqual (true, VCDiffSharp.VCDiff.Decode ("target.txt", "patch.txt", "output.txt"));
		}
	}
}
