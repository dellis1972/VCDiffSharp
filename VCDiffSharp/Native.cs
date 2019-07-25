using System;
using System.IO;
using System.Runtime.InteropServices;

namespace VCDiffSharp {
	class Native {

		[DllImport ("vc-diff")]
		internal static extern int Encode (string sourceFile, string targetFile, string patchFile);

		[DllImport ("vc-diff")]
		internal static extern int Decode (string targetFile, string patchFile, string outputFile);
	}

	/// <summary>
	/// 
	/// </summary>
	public class VCDiff {

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sourceFile"></param>
		/// <param name="targetFile"></param>
		/// <param name="patchFile"></param>
		/// <returns></returns>
		public static bool Encode (string sourceFile, string targetFile, string patchFile)
		{
			if (!File.Exists (sourceFile))
				throw new FileNotFoundException (sourceFile);
			if (!File.Exists (targetFile))
				throw new FileNotFoundException (targetFile);
			if (File.Exists (patchFile))
				throw new IOException ($"{patchFile} already exists.");


			return Native.Encode (sourceFile, targetFile, patchFile) == 0;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="targetFile"></param>
		/// <param name="patchFile"></param>
		/// <param name="outputFile"></param>
		/// <returns></returns>
		public static bool Decode (string targetFile, string patchFile, string outputFile)
		{
			if (!File.Exists (targetFile))
				throw new FileNotFoundException (targetFile);
			if (!File.Exists (patchFile))
				throw new FileNotFoundException (patchFile);
			if (File.Exists (outputFile))
				throw new IOException ($"{outputFile} already exists.");
			return Native.Decode (targetFile, patchFile, outputFile) == 0;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="source"></param>
		/// <param name="target"></param>
		/// <param name="patch"></param>
		/// <returns></returns>
		public static bool EncodeStream (Stream source, Stream target, Stream patch)
		{
			return true;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="target"></param>
		/// <param name="patch"></param>
		/// <param name="output"></param>
		/// <returns></returns>
		public static bool DecodeStream (Stream target, Stream patch, Stream output)
		{
			return true;
		}
	}
}
