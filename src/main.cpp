/*
1. Implement Grid Class
2. Implement a GUI
3. Implement an equation compiler
*/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "gl.h"
#include "gui_manager.h"
#include "shader.h"
#include "glbuffer.h"
#include "glvertexarray.h"
#include <filesystem>
#include "texture.h"
#include "renderer.h"
#include "util.h"
#include "gpu_allocator.h"
#include "geometry.h"
#include "grid.h"
#include "utility"
#include "vertices.h"
#include "matharena.h"
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
extern Profiler profiler;
extern Renderer renderer;
extern Logger logger;
extern GUI GUI_Manager;
extern MathArena* matharena;
GLManager gl;
ShaderProgram* spGlobal;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;






int main()
{
    printf("here\n");
    // Check for ARB_invalidate_subdata support
    if (glInvalidateBufferSubData == NULL) {
        // Handle the case where the extension is not supported
        printf("not supported\n");
        return 0;
    }

    //GUI_Manager.RegisterLogTarget(&logger);
    logger.SetLogLevel(LOGLEVEL::LEVEL_INFO);
    logger.Log(LOGTYPE::INFO, "test");
    


    matharena = new MathArena();
    if (matharena->HasError())
    {
        
    }



    gl.SetDepthTesting(true);
    gl.SetStencilTesting(true);

    Shader vertex_shader(util::getcwd() + "/src/shaders/vertex.glsl", GL_VERTEX_SHADER);
    if (vertex_shader.CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, vertex_shader.FetchLog());


    
    Shader fragment_shader(util::getcwd() + "/src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    if (fragment_shader.CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, fragment_shader.FetchLog());





    // NORMALS SHADER
    Shader geometry_vertex_shader(util::getcwd() + "/src/shaders/vertex_normals.glsl", GL_VERTEX_SHADER);
    if (geometry_vertex_shader.CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, geometry_vertex_shader.FetchLog());
    Shader geometry_fragment_shader(util::getcwd() + "/src/shaders/fragment_normals.glsl", GL_FRAGMENT_SHADER);
    if (geometry_fragment_shader.CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, geometry_fragment_shader.FetchLog());

    Shader geometry_geometry_shader(util::getcwd() + "/src/shaders/geometry_normals.glsl", GL_GEOMETRY_SHADER);
    if (geometry_geometry_shader.CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, geometry_geometry_shader.FetchLog());
    

    ShaderProgram normalSP;
    normalSP.AddShader(&geometry_vertex_shader);
    normalSP.AddShader(&geometry_fragment_shader);
    normalSP.AddShader(&geometry_geometry_shader);

    // check for shader compile errors
    ShaderProgram shaderProgram;
    shaderProgram.AddShader(&vertex_shader);
    shaderProgram.AddShader(&fragment_shader);

    spGlobal = &shaderProgram;
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    renderer.SetLightingModel(LightingModel::Phong);


   


    if (!shaderProgram.Compile() || !normalSP.Compile())
    {
         
        normalSP.CheckError();
        while(1);
        return -1;
    }









    BufferLayout* lighting_layout = new BufferLayout({new BufferElement("COORDS", ShaderDataType::Float3, false),
         new BufferElement("NORMALS", ShaderDataType::Float3, false),
          new BufferElement("TEXCOORDS", ShaderDataType::Float2, false) });
    VertexArray* lva = new VertexArray;
    VertexBuffer* lighting_vbo = new VertexBuffer(tvertices, sizeof(tvertices));
    lighting_vbo->SetLayout(lighting_layout);
    lva->AddVertexBuffer(lighting_vbo);
    lva->SetCount(36);

    RenderObject* l_obj = new RenderObject(lva, lighting_vbo, &shaderProgram, OBJECTYPE::PointLightSource);
    l_obj->SetPosition({0.0f, 0.0f, 0.0f});
    l_obj->m_Light.direction = glm::vec3({0.0, 0.0, 0.0});



    //Chunk* chunk = new Chunk(0, 0, ChunkManager::m_ChunkShader);
    //renderer.AddRenderObject(chunk->GetRenderObject());    

    //BufferLayout* tex_layout = new BufferLayout({new BufferElement("COORDS", ShaderDataType::Float3, false),
    //      new BufferElement("NORMALS", ShaderDataType::Float3, false),new BufferElement("TEXCOORDS", ShaderDataType::Float2, false) });
    //VertexArray* tva = new VertexArray;
    //IndexBuffer* iva = new IndexBuffer((uint32_t*)cubeIndices.data(), BLOCK_INDICES_COUNT);
    //VertexBuffer* tex_vbo = new VertexBuffer((float*)dirt_vertices.data(), BLOCK_VERTICES_SIZE);
    //tex_vbo->SetLayout(tex_layout);
    //tva->AddVertexBuffer(tex_vbo);
    //tva->AddIndexBuffer(iva);
    //RenderObject* t_obj = new RenderObject(tva, tex_vbo, &shaderProgram, iva, OBJECTYPE::TexturedObject);
    //l_obj->SetPosition({0.0, 0.0, 0.0});
   // renderer.AddRenderObject(l_obj);

    
    

    

    //renderer.AddRenderObject(t_obj);
    //renderer.AddRenderObject(l_obj);

    for (int i = -20; i < 20; i += 5)
    {
        for (int j = -20; j < 20; j += 5)
        {
            auto obj = l_obj->Duplicate();
            obj->Translate({1.0f*i, 95.0f, 1.0f*j});
            renderer.AddRenderObject(obj);
        }
    }
    
    //l_obj->Translate(glm::vec3(0.0f, 90.0f, 0.0f));
    //renderer.AddRenderObject(l_obj);
//  
    //auto sphere = new Sphere(1.0f, 100, &shaderProgram);
    glLineWidth(2);

   //auto sphere = new Function3D(8.0f, 12.0f, 8.0f, 500, &shaderProgram);

  //  auto sphereNormals = new Function3D(8.0f, 12.0f, 8.0f, 500, &normalSP);
    printf("created sphere\n");
    //renderer.AddRenderObject(sphere->m_RenderObj);
   // renderer.AddRenderObject(sphereNormals->m_RenderObj);
    //renderer.AddRenderObject(grid->m_RenderObj);

    //auto 


    //for (int i = 0; i < 16; i += 1)
    //{
    //    for (int j = 0; j < 16; j += 1)
    //    {
    //        for (int z = 0; z > -64; z -= 1)
    //        {
    //            auto xobj = t_obj->Duplicate();
    //            xobj->Translate({i*1.0f, z*1.0f, j*1.0f});
    //            renderer.AddRenderObject(xobj);
    //        }
    //    }
    //    
    //}
                
    

   
    
    
    

    // va.AddIndexBuffer(ibo);
   //Texture tex(getcwd() + "/src/textures/container.jpg", "container");





    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(gl.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    float opacity = 0.5f;
    bool sign = false;

    // /shaderProgram.Bind();
    // /if(!shaderProgram.SetUniform1i("texture1", 0))
    // /{
    // /    printf("Could not bind texture1\n");
    // /}

    //ModelImporter* import = new ModelImporter(&shaderProgram);
    //import->LoadModel("/home/cole/Documents/voxel-engine/include/dev/models/mech_tank/scene.gltf");
    //auto mesh_model = import->ExportCurrentModel();
    //auto obj = new RenderObject(&shaderProgram, mesh_model);
    //for (int i = 0; i < 2; i++)
    //{
    //    auto xobj = obj->Duplicate();
    //    xobj->SetPosition(glm::vec3(50 *util::Random(), 50 *util::Random(), 50 *util::Random()));
    //    xobj->Rotate(glm::vec3(util::Random(), util::Random(), util::Random()), 180.0f * util::Random());
    //    renderer.AddRenderObject(xobj);
    //}


   
    
   
    while (!glfwWindowShouldClose(gl.GetWindow()))
    {
        //printf("here\n");
        // gl.CalcDeltaTime();
       // chunkManager.PerFrame();
        gl.PerFrame();
        logger.WriteLogs();
         
        // model = glm::rotate(model, .1f, );
       // r_obj.Rotate(glm::vec3(0.5f, 1.0f, 0.0f), .1f);
        // float time = glfwGetTime();

        // auto new_model = glm::translate(model, glm::vec3(cos(time), 0, 0));
        // input
        // -----
        // processInput(gl.GetWindow(), view);

        // render
        // ------

        // tell OpenGL a new frame is about to begin
        GUI_Manager.Begin();

        // draw our first triangle
        // glUseProgram(shaderProgram);

        // tex.SetTextureSlot(0);
        // tex.Bind();

       // shaderProgram.Bind();
       // shaderProgram.SetUniformVec3("lightColor", lightColor);
       // shaderProgram.SetUniformVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
       // shaderProgram.SetUniformVec3("lightPos", ls_pos);
       // shaderProgram.SetUniformVec3("viewPos", gl.GetCamera()->GetPosition());
        // shaderProgram.SetUniformMat4("model", model);
        
       


        // va.Bind();
      
       
        
        renderer.RenderAllObjects();
        //gl.GetCamera()->CameraHandleKey_W();
        GUI_Manager.RenderGUI();
        
        // Text that appears in the window
        // ImGui::Text("Hello there adventurer!");

        // ImGui::End();

        GUI_Manager.End();

        // va.Bind();
        // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

      //  lightSource.Bind();
      //  lightSource.SetUniformMat4("model", ls_model);
      //  lightSource.SetUniformVec3("lightColor", lightColor);

       // if (!gl.UpdateCameraMVP(&lightSource))
       // {
       //     std::cout << gl.GetCamera()->FetchLog() << std::endl;
       //     exit(-1);
       // }
//
       // lva.Bind();
       // glDrawArrays(GL_TRIANGLES, 0, 36);

        // glBindVertexArray(0); // no need to unbind it every time
        
        
        //GUI_Manager.RenderGUI();
        
        // Text that appears in the window
        // ImGui::Text("Hello there adventurer!");

        // ImGui::End();

        //GUI_Manager.End();
        // Renders the ImGUI elements
        // ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        auto swapBufferToken = new PROFILER_TOKEN("glfwSwapBuffers");
        glfwSwapBuffers(gl.GetWindow());
        delete swapBufferToken;
        auto pollEventsToken = new PROFILER_TOKEN("glfwPollEvents");
        glfwPollEvents();
        delete pollEventsToken;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}