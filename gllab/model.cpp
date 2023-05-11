#include "model.hpp"
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "static.hpp"
#include "globals.hpp"



#define BUFFER_OFFSET(i) ((char *)NULL + (i))


GameModel::GameModel() {
    model_mat = glm::mat4(1.0f);
    model_rot = glm::mat4(1.0f);
    model_pos = glm::vec3(-3, 0, -3);
    view_mat = genView(glm::vec3(2, 2, 20), model_pos);
    modelData = std::make_unique<tinygltf::Model>();
}
GameModel::~GameModel() {
}


bool GameModel::loadFromFile(const std::string filename) {

    std::string err;
    std::string warn;

    bool res = Static::loader.LoadASCIIFromFile(modelData.get(), &err, &warn, filename);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;
    else
        std::cout << "Loaded glTF: " << filename << std::endl;

    return res;
}



void GameModel::bindMesh(tinygltf::Mesh& mesh, const tinygltf::Node& node) {
    meshData = calculatePrimitiveBufferParams(mesh);


    for (size_t i = 0; i < mesh.primitives.size(); ++i) {

        auto& shaderManager = Globals::shaderManager;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        auto& prim = meshData[i];
        prim.vao = vao;
        prim.currentBufferOffset = 0;

        const auto& primitive = mesh.primitives[i];

        const auto& material = modelData->materials[primitive.material];
        if (modelData->textures.size() > 0)
        {

            glGenTextures(1, &prim.texture);
            glBindTexture(GL_TEXTURE_2D, prim.texture);



            const auto& texture = modelData->textures[material.pbrMetallicRoughness.baseColorTexture.index];
            const auto& image = modelData->images[texture.source];
            const auto& sampler = modelData->samplers[texture.sampler];

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.magFilter);
            

            GLenum format = GL_RGBA;

            if (image.component == 1) {
                format = GL_RED;
            }
            else if (image.component == 2) {
                format = GL_RG;
            }
            else if (image.component == 3) {
                format = GL_RGB;
            }
            else {
                // ???
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, image.pixel_type, &image.image.at(0));
            glGenerateMipmap(GL_TEXTURE_2D);
        }


        prim.shader = &shaderManager.getShaderByMaterial(material.name.c_str());


        for (size_t j = 0; j < modelData->bufferViews.size(); ++j) {
            const tinygltf::BufferView& bufferView = modelData->bufferViews[j];
            if (bufferView.target == 0) {  // TODO impl drawarrays
                std::cout << "WARN: bufferView.target is zero" << std::endl;
                continue;  // Unsupported bufferView.

            }

            const tinygltf::Buffer& buffer = modelData->buffers[bufferView.buffer];
            std::cout << "bufferview.target " << bufferView.target << std::endl;

            GLuint vbo;
            glGenBuffers(1, &vbo);
            prim.bufData[j].vbo = vbo;
            glBindBuffer(bufferView.target, vbo);

            std::cout << "accessor data size = " << prim.bufData[j].accessorByteLength
                << ", bufferview + accessor byteOffset = " << bufferView.byteOffset + prim.bufData[j].accessorByteOffset
                << "buffer data at 0" << buffer.data.at(0) 
                << std::endl;
            
            glBufferData(bufferView.target, prim.bufData[j].accessorByteLength,
                &buffer.data.at(0) + bufferView.byteOffset + prim.bufData[j].accessorByteOffset, GL_DYNAMIC_DRAW);
            prim.currentBufferOffset += prim.bufData[j].accessorByteLength;

        }

        for (auto& attrib : primitive.attributes) {
            tinygltf::Accessor accessor = modelData->accessors[attrib.second];
            int byteStride =
                accessor.ByteStride(modelData->bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, prim.bufData[accessor.bufferView].vbo);

           

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
            if (vaa > -1) {

                std::cout << "vbo: " << prim.bufData[accessor.bufferView].vbo << "\n" <<
                    "vaa: " << vaa << "\n" <<
                    "size: " << size << "\n" <<
                    "type: " << accessor.componentType << "\n" <<
                    "norm: " << accessor.normalized << "\n"
                    "stride: " << byteStride << "\n" <<
                    "byteoffset: " << accessor.byteOffset << "\n" << std::endl;

                glEnableVertexAttribArray(vaa);

                glVertexAttribPointer(vaa, size, accessor.componentType,
                    accessor.normalized ? GL_TRUE : GL_FALSE,
                    byteStride, BUFFER_OFFSET(0));


            }
            else
                std::cout << "vaa missing: " << attrib.first << std::endl;
        }

        glBindVertexArray(0);
    }

}

std::map<int, GameModel::PrimitiveData> GameModel::calculatePrimitiveBufferParams(tinygltf::Mesh& mesh) {

    std::map<int, PrimitiveData> meshData;

    for (size_t i = 0; i < mesh.primitives.size(); i++)
    {
        PrimitiveData primData;
        auto primitive = mesh.primitives[i];
              

        for (auto& attrib : primitive.attributes)
        {
            auto accessor = modelData->accessors[attrib.second];

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;

            if (vaa < 0)
            {
                std::cout << "Unsupported buffer type" << "\n";
            }
            else
            {
                primData.bufData[vaa].bufferView = attrib.second;
                primData.bufData[vaa].accessorByteOffset = accessor.byteOffset;
                primData.bufData[vaa].accessorByteLength = accessor.count * accessor.ByteStride(modelData->bufferViews[accessor.bufferView]); //does not support interleaved data
            }
            
        }
       

        tinygltf::Accessor indexAccessor = modelData->accessors[primitive.indices];
        primData.bufData[Static::vaamax + 1].bufferView = indexAccessor.bufferView;
        primData.bufData[Static::vaamax + 1].accessorByteOffset = indexAccessor.byteOffset;
        primData.bufData[Static::vaamax + 1].accessorByteLength = indexAccessor.count * indexAccessor.ByteStride(modelData->bufferViews[indexAccessor.bufferView]);

        meshData[i] = primData;
    }

    return meshData;
    
}

// bind models
void GameModel::bindModelNodes(tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < modelData->meshes.size())) {

        bindMesh(modelData->meshes[node.mesh], node);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < modelData->nodes.size()));
        bindModelNodes(modelData->nodes[node.children[i]]);
    }
}

void GameModel::bind() {

    const tinygltf::Scene& scene = modelData->scenes[modelData->defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < modelData->nodes.size()));
        bindModelNodes(modelData->nodes[scene.nodes[i]]);
    }

}

glm::mat4 GameModel::calculateModelMat(const tinygltf::Node& node, const Transform& transform) const{
    glm::mat4 modelmat;
    if (node.matrix.size() > 0)
    {
        modelmat = glm::mat4(
            node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
            node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7],
            node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11],
            node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]);
    }
    else
    {        
        modelmat = glm::mat4(1.0f);

        if (node.scale.size() > 0)
        {
            modelmat = glm::scale(modelmat, glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
        }
        
        if (node.rotation.size() > 0)
        {
            modelmat *= glm::mat4_cast(glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]));
        }
        
        if (node.translation.size() > 0)
        {
            modelmat = glm::translate(modelmat, glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
        }
        

    }
    
    modelmat *= glm::mat4(transform.getTranslateMat());
    modelmat *= glm::mat4(transform.getRotationMat());
    modelmat *= glm::mat4(transform.getScaleMat());

    return modelmat;
    
}

void GameModel::setColliderFromMesh()
{
    collisionShape = std::make_unique<btBoxShape>(btVector3(btScalar(0.5), btScalar(0.5), btScalar(0.5)));
}

void GameModel::setCustomCollider(btVector3 dimensions)
{
    collisionShape = std::make_unique<btBoxShape>(dimensions);
    
    //collisionShape->setMargin(0.2);
    
   
}

btCollisionShape* GameModel::getCollisionShape()
{
    return collisionShape.get();
}

void GameModel::drawMesh(tinygltf::Mesh& mesh, const Camera& camera, const tinygltf::Node& node, const Transform& transform) const{
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        const auto& prim = meshData.at(i);
        const tinygltf::Primitive& primitive = mesh.primitives[i];
        const tinygltf::Accessor& indexAccessor = modelData->accessors[primitive.indices];

        glBindVertexArray(prim.vao);

        prim.shader->use();
        
        prim.shader->setMat4("projection", camera.projection());
        prim.shader->setMat4("view", camera.view());
        prim.shader->setMat4("model", calculateModelMat(node, transform));
        const auto& scale = transform.getScale();

        if (scale.x * scale.y * scale.z < 0)
        {
            glFrontFace(GL_CW);
        }
        else
        {
            glFrontFace(GL_CCW);
        }

        const auto& baseclr = modelData->materials[primitive.material].pbrMetallicRoughness.baseColorFactor;
        prim.shader->setVec4("baseColorFactor", glm::vec4(baseclr[0], baseclr[1], baseclr[2], baseclr[3]));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prim.bufData.at(indexAccessor.bufferView).vbo);

        if (modelData->textures.size() > 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, prim.texture);
        }

        glDrawElements(primitive.mode, indexAccessor.count,
            indexAccessor.componentType,
            BUFFER_OFFSET(0));

        glBindVertexArray(0);
    }
}

// recursively draw node and children nodes of model
void GameModel::drawModelNodes(tinygltf::Node& node, const Camera& camera, const Transform& transform) const {
    if ((node.mesh >= 0) && (node.mesh < modelData->meshes.size())) {
        drawMesh(modelData->meshes[node.mesh], camera, node, transform);
    }
    for (size_t i = 0; i < node.children.size(); i++) {
        drawModelNodes(modelData->nodes[node.children[i]], camera, transform);
    }
}

void GameModel::draw(const Camera& camera) const {
    draw(camera, Transform());
}

void GameModel::draw(const Camera& camera, const Transform& transform) const {

    const tinygltf::Scene& scene = modelData->scenes[modelData->defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        drawModelNodes(modelData->nodes[scene.nodes[i]], camera, transform);
    }

}

void GameModel::dbgModel() {
    for (auto& mesh : modelData->meshes) {
        std::cout << "mesh : " << mesh.name << std::endl;
        for (auto& primitive : mesh.primitives) {
            const tinygltf::Accessor& indexAccessor =
                modelData->accessors[primitive.indices];

            std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
                << indexAccessor.componentType << std::endl;

            tinygltf::Material& mat = modelData->materials[primitive.material];
            for (auto& mats : mat.values) {
                std::cout << "mat : " << mats.first.c_str() << std::endl;
            }

            for (auto& image : modelData->images) {
                std::cout << "image name : " << image.uri << std::endl;
                std::cout << "  size : " << image.image.size() << std::endl;
                std::cout << "  w/h : " << image.width << "/" << image.height
                    << std::endl;
            }

            std::cout << "indices : " << primitive.indices << std::endl;
            std::cout << "mode     : "
                << "(" << primitive.mode << ")" << std::endl;

            for (auto& attrib : primitive.attributes) {
                std::cout << "attribute : " << attrib.first.c_str() << std::endl;
            }
        }
    }
}

glm::mat4 GameModel::genView(glm::vec3 pos, glm::vec3 lookat) {
    // Camera matrix
    glm::mat4 view = glm::lookAt(
        pos,                // Camera in World Space
        lookat,             // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    return view;
}

glm::mat4 GameModel::genMVP(glm::mat4 view_mat, glm::mat4 model_mat, float fov, int w,
    int h) {
    glm::mat4 Projection =
        glm::perspective(glm::radians(fov), (float)w / (float)h, 0.01f, 1000.0f);

    // Or, for an ortho camera :
    // glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
    // // In world coordinates

    glm::mat4 mvp = Projection * view_mat * model_mat;

    return mvp;
}

double GameModel::getScalingFactorFromAccessors() const
{
    double max = std::numeric_limits<double>::min(), min = std::numeric_limits<double>::max();
    for (const auto& accessor: modelData->accessors)
    {
        for (const auto& maxvalue : accessor.maxValues)
        {
            if (maxvalue > max)
            {
                max = maxvalue;
            }
        }

        for (const auto& minvalue : accessor.minValues)
        {
            if (minvalue < min)
            {
                min = minvalue;
            }
        }
        
    }

    if (abs(min) > max)
    {
        max = min;
    }

    return 0.5 / max;
}
