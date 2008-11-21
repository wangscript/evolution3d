using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using xEvol3D;
namespace BaseTest.Net
{

    public partial class Form1 : Form
    {
        private class CanvasMouseState
        {
            public int last_x;
            public int last_y;
            public MouseButtons btn;
        };
        xEvol3DEnv          m_Evol3D;
        xRenderApi          m_RenderApi;
        xCamera             m_2DCamera;
        xCamera             m_3DCamera;
        HBaseTexture        m_MaskTexture;
        HBaseTexture        m_DiffuseTexture;
        xGpuProgram         m_pRectShader;
        xGpuProgram         m_pRectBlankShader;
        xBlenderState       m_DefBlender;
        xBaseTextureManager m_TexMgr;
        xBaseModelManager   m_modelMgr;

        xBaseModel          m_model;
        xBaseModel          m_scene;

        
        xGpuProgram         m_SkelShader;
        xGpuProgram         m_pModelShader;
        float               m_Angle;

        CanvasMouseState m_CanvasMouseState;
        public Form1()
        {
            InitializeComponent();
            m_Evol3D = new xEvol3DEnv();
            m_Angle = 0;
            m_CanvasMouseState = new CanvasMouseState();
            //m_CanvasMouseState.last_x = new int();
            //m_CanvasMouseState.last_y = new int();
            m_CanvasMouseState.last_x = 0;
            m_CanvasMouseState.last_y = 0;
            m_CanvasMouseState.btn = MouseButtons.None;
            Application.Idle += OnIdle;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_Evol3D.Init(".\\system.xml", 0 , (int)Canvas.Handle, ".\\");
            m_RenderApi = m_Evol3D.createRenderApi();
            m_2DCamera = m_RenderApi.createCamera("2DCamera");
            m_3DCamera = m_RenderApi.createCamera("3DCamera");
            

 
            m_TexMgr = new xBaseTextureManager(m_RenderApi, "DefTexMgr" , false);
            m_TexMgr.addPath(m_Evol3D.absPath("texture/"));

            m_modelMgr = new xBaseModelManager(m_RenderApi, m_TexMgr, "ModelMgr");
            m_modelMgr.addPath(m_Evol3D.absPath("model/"));

            m_model = m_modelMgr.loadModel("Girl-Evol.xrm");
            m_scene = m_modelMgr.loadModel("Scene.xrm");
            xVector3 min = new xVector3();
            xVector3 max = new xVector3();
            m_model.BoundBox(min ,max);

            //xVector3 scenH = new xVector3();
            //xVector3 scenL = new xVector3();
            //m_scene.BoundBox(scenL, scenH);

            xVector3 len = new xVector3(max.x - min.x , max.y - min.y , max.z - min.z );
            xVector3 cen = new xVector3(0.5f * max.x + 0.5f * min.x, 0.5f * max.y + 0.5f * min.y, 0.5f * max.z + 0.5f * min.z);

            m_3DCamera.EyePos = new xVector(cen.x + 2.0f * len.x, cen.y, cen.z, 1.0f);
            m_3DCamera.EyeTarget = new xVector(cen.x, cen.y, cen.z, 1.0f);
            m_3DCamera.EyeUp = new xVector(0.0f, 0.0f, 1.0f, 1.0f);
            m_3DCamera.fov = 45.0f;
            m_3DCamera.CameraStep = len.x / 100.0f;
            
            m_3DCamera.nearPlan = 0.1f;
            m_3DCamera.farPlan = len.z * 2.0f;
            m_3DCamera.projType = eCameraProject.PT_PROJECT;

            ResizeRenderBuffer();


            m_DefBlender = m_RenderApi.createBlendState("Font.Blend");
            m_pRectShader = m_RenderApi.loadGpuProgram("simple2D.vertex", "simple2D.pixel<0:simple.texture,simple.mask>", "");
            m_pRectBlankShader = m_RenderApi.loadGpuProgram("simple2D.vertex", "simple2D.pixel", "");
            m_pModelShader = m_RenderApi.loadGpuProgram("simpleMesh.vertex", "simpleMesh.pixel<0:simple.texture;>", "");
            m_SkelShader = m_RenderApi.loadGpuProgram("simpleMesh.vertex<0:SkinAni;>", "simpleMesh.pixel<0:simple.texture;>", "");
            m_DiffuseTexture = m_TexMgr.load("Cover.jpg");
            m_MaskTexture    = m_TexMgr.load("Mask.png");
            m_ObjectProperty.SelectedObject = m_3DCamera;
        }

        private void OnIdle(object sender, EventArgs e)
        {
            m_RenderApi.identityMatrix(eMatrixMode.MATRIXMODE_World);
            m_RenderApi.begin(0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0);
            m_RenderApi.beginScene();
            m_RenderApi.applyCamera(m_3DCamera);
            m_RenderApi.setBlendState(m_DefBlender);
            
            xDepthStencilState depStencil = new xDepthStencilState();
            m_RenderApi.setDepthStencilState(depStencil);

            m_RenderApi.GpuShader = m_pModelShader;

            xVector3 min = new xVector3();
            xVector3 max = new xVector3();
            m_scene.BoundBox(min, max);

            xVector3 len = new xVector3(max.x - min.x, max.y - min.y, max.z - min.z);
            xVector3 cen = new xVector3(0.5f * max.x + 0.5f * min.x, 0.5f * max.y + 0.5f * min.y, 0.5f * max.z + 0.5f * min.z);

            //设置矩阵绘制场景
            xMatrix tranMat = new xMatrix();
            xMathlib.Transform(tranMat, -cen.x, -cen.y, -cen.z);
            m_RenderApi.setMatrix(tranMat, eMatrixMode.MATRIXMODE_World);            
            m_scene.Draw();
            int timePassed = 20;
            m_RenderApi.GpuShader = m_SkelShader;

            //单位矩阵绘制角色
            m_RenderApi.identityMatrix(eMatrixMode.MATRIXMODE_World);
            m_model.update(timePassed);
            m_model.Draw();
            xColorF cl  = new xColorF(1.0f , 1.0f , 1.0f ,1.0f );
            xMatrix mat = new xMatrix();
            mat.identity();
            m_RenderApi.setMatrix(mat, eMatrixMode.MATRIXMODE_World);
            //cl.r = 1.0f; cl.g = 1.0f; cl.b = 0.0f; cl.a = 1.0f;
            m_RenderApi.applyCamera(m_2DCamera);
            
            m_RenderApi.GpuShader = m_pRectShader;
            xVector destRect = new xVector();
            destRect.left = 10; destRect.top = 400; destRect.width = 400; destRect.height = 300;

            m_RenderApi.setBlendState(m_DefBlender);
            m_RenderApi.set2DTexture( 1  , m_MaskTexture );
            m_RenderApi.drawRectf(m_DiffuseTexture , destRect , cl);
            m_RenderApi.GpuShader = m_pRectBlankShader;

            m_Angle += 0.5f;
            destRect.left = 500; destRect.top = 10; destRect.width = 400; destRect.height = 300;
            xMatrix rotMat = new xMatrix();
            xMathlib.RotateZ(rotMat, m_Angle);
            xMatrix xformMat = new xMatrix();
            xMathlib.XForm_Trans(xformMat , rotMat , new xVector3( (destRect.left + destRect.width/2.0f) , (destRect.top + destRect.height/2.0f)  , 0.0f ) );
            m_RenderApi.setMatrix(xformMat,eMatrixMode.MATRIXMODE_World);
            //m_RenderApi.drawRectf(destRect, new xColorF(1.0f, 0.0f, 0.0f, 1.0f) );

            m_RenderApi.identityMatrix(eMatrixMode.MATRIXMODE_World);
            m_RenderApi.findFont("default").drawText("Evolution3D 演示 D3D10",10,50,cl);
            m_RenderApi.endScene();
            m_RenderApi.swapBuffer();

            
        }

        public void ResizeRenderBuffer()
        {
            m_Evol3D.ResizeRenderWindow(Canvas.Width, Canvas.Height);
            m_2DCamera.setPerspective2D(Canvas.Width, Canvas.Height, 60, true);
            m_RenderApi.onResize(Canvas.Width, Canvas.Height);
            m_3DCamera.aspect = (float)(Canvas.Width) / (float)Canvas.Height;
            m_2DCamera.setPerspective2D(Canvas.Width, Canvas.Height, 60, true);
            m_ObjectProperty.SelectedObject = m_3DCamera;
        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            ResizeRenderBuffer();
        }


         private void Left_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.yaw(-1);
        }

         private void Right_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.yaw(1);
        }

         private void Back_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.toward(-30);
        }

         private void Forword_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.toward();
        }


        private void Up_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.up();
        }

        private void Down_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.down();
        }

        private void PitchDown_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.pitch(1);
        }

        private void PitchUp_MouseMove(object sender, MouseEventArgs e)
        {
            m_3DCamera.pitch(-1);
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void Canvas_MouseMove(object sender, MouseEventArgs e)
        {
            if(m_CanvasMouseState.btn == MouseButtons.Right)
            {
                int dx = e.X - m_CanvasMouseState.last_x;
                int dy = e.Y - m_CanvasMouseState.last_y;
                if( Math.Abs(dx) > Math.Abs(dy) )
                {
                    float angle = 90 * (float)dx / (float)Canvas.Width;
                    m_3DCamera.yaw(angle);
                }
                else
                {
                    float angle = 90 * (float)dy / (float)Canvas.Height;
                    m_3DCamera.pitch(angle);
                }
                m_CanvasMouseState.last_x = e.X;
                m_CanvasMouseState.last_y = e.Y;
            }
            
        }

        private void Canvas_MouseDown(object sender, MouseEventArgs e)
        {
            m_CanvasMouseState.btn = e.Button;
            m_CanvasMouseState.last_x = e.X;
            m_CanvasMouseState.last_y = e.Y;
        }

        private void Canvas_MouseUp(object sender, MouseEventArgs e)
        {
            m_CanvasMouseState.btn = MouseButtons.None;
            m_CanvasMouseState.last_x = e.X;
            m_CanvasMouseState.last_y = e.Y;

        }

        private void Canvas_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if(e.KeyCode == Keys.W )
            {
                m_3DCamera.toward();
            }
            else if(e.KeyCode == Keys.S)
            {
                m_3DCamera.back();
            }
            else if (e.KeyCode == Keys.A)
            {
                m_3DCamera.shiftLeft();
            }
            else if (e.KeyCode == Keys.D)
            {
                m_3DCamera.shiftRight();
            }
            else if (e.KeyCode == Keys.Left)
            {
                m_3DCamera.yaw(-1);
            }
            else if (e.KeyCode == Keys.Right)
            {
                m_3DCamera.yaw(1);
            }

        }



        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.W)
            {
                m_3DCamera.toward();
            }
            else if (e.KeyCode == Keys.S)
            {
                m_3DCamera.back();
            }
            else if (e.KeyCode == Keys.A)
            {
                m_3DCamera.shiftLeft();
            }
            else if (e.KeyCode == Keys.D)
            {
                m_3DCamera.shiftRight();
            }
            else if (e.KeyCode == Keys.Left)
            {
                m_3DCamera.yaw(-1);
            }
            else if (e.KeyCode == Keys.Right)
            {
                m_3DCamera.yaw(1);
            }

        }
    }
}
