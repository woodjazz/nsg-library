#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "QtQuickItem.h"
#include "NSG.h"
using namespace NSG;

int main(int argc, char **argv)
{
    PAnimationController acontrol;
    auto window = Window::CreateExternal();
    LoaderXML loader("loader");
    auto slotLoaded = loader.Load(Resource::GetOrCreateClass<ResourceFile>("data/duck.xml"))->Connect([&]()
    {
        auto scene = loader.GetScene(0);
        auto objNode = scene->GetOrCreateChild<SceneNode>("LOD3sp");
        auto objPos = objNode->GetGlobalPosition();
        auto objBB = objNode->GetWorldBoundingBox();
        objBB.max_ *= 1.75f;
        objBB.min_ *= 1.75f;
        auto camera = scene->GetOrCreateChild<Camera>("camera1");
        camera->SetWindow(window);
        auto control = std::make_shared<CameraControl>(camera);

        auto animation = Animation::Create("anim0");
        AnimationTrack track;
        track.node_ = camera;
        track.channelMask_ = (int)AnimationChannel::POSITION | (int)AnimationChannel::ROTATION;

        {
            AnimationKeyFrame key(0, camera.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node0");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(objBB.max_.x, objBB.max_.y, 0));
            node->SetGlobalLookAtPosition(objPos);
            AnimationKeyFrame key(2, node.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node1");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(0, objBB.max_.y, objBB.min_.z));
            node->SetGlobalLookAtPosition(objPos);
            AnimationKeyFrame key(4, node.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node2");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(objBB.min_.x, objBB.max_.y, 0));
            node->SetGlobalLookAtPosition(objPos);
            AnimationKeyFrame key(6, node.get());
            track.keyFrames_.push_back(key);
        }

        animation->AddTrack(track);
        animation->SetLength(8);

        acontrol = std::make_shared<AnimationController>(camera);
        acontrol->Play(animation->GetName(), false);
        window->SetScene(scene);
    });

    auto engine = Engine::Create();

    QGuiApplication app(argc, argv);
    qmlRegisterType<QtQuickItem>("OpenGLUnderQML", 1, 0, "MyQMLItem");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qtQuick/qtQuick/main.qml"));
    view.show();
    return app.exec();
}
