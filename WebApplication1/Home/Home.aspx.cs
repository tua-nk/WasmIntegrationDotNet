using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication1.WasmPages
{

    public partial class Home : System.Web.UI.Page
    {
        public static int MAX_SRC_LINES_GEN = 100000;
        protected void Page_Load(object sender, EventArgs e)
        {
            bool bTest = false;
            if (bTest)
            {
                CreateTestSrcFile();
            }
        }

        public void CreateTestSrcFile()
        {
            String srcFilePath = GetSrcFilePath("BigCppFunc");
            if (!File.Exists(srcFilePath))
            {
                // Create a file to write to.
                using (StreamWriter sw = File.CreateText(srcFilePath))
                {
                    sw.WriteLine("void bigFunc(unsigned int nSeedVal)");
                    sw.WriteLine("{");
                    for (UInt32 uiLineNum = 0; uiLineNum < MAX_SRC_LINES_GEN; uiLineNum++)
                    {
                        sw.WriteLine(String.Format("   intVal+= {0} + nSeedVal;", uiLineNum));
                    }
                    sw.WriteLine("}");
                }
            }
        }

        public static String SRCGEN_FOLDER = "/SrcGen/";
        public static String GetSrcFilePath(String srcBaseFileName)
        {
            String urlDirectoryPath = String.Format("{0}", SRCGEN_FOLDER);
            String serverDirectoryPath = System.Web.HttpContext.Current.Server.MapPath(urlDirectoryPath);

            try
            {
                DirectoryInfo info = Directory.CreateDirectory(serverDirectoryPath);
            }
            catch (Exception e)
            {

            }
            String urlFilePath = String.Format("{0}/{1}.cpp", SRCGEN_FOLDER, srcBaseFileName);
            String serverPath = System.Web.HttpContext.Current.Server.MapPath(urlFilePath);
            return serverPath;
        }
    }
}