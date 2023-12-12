#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "gazebo/common/UpdateInfo.hh"
#include <jsoncpp/json/json.h>
#include <fstream>

namespace gazebo
{
  class DynamicJointModelPlugin : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->sdf = _sdf;
      this->poses_keyframes = std::vector<Pose>();
      this->curr_pose = 0.0;

      // physics::Model robot_model = physics::Model();

      // physics::BasePtr world = _parent->GetParent();
      this->world = physics::get_world();
      std::string my_name = "robile_john";
      // std::cout << "WORLD >>>>>>>>>> " << world->Name() << std::endl;

      // std::vector<physics::ModelPtr> models = world->Models();
      // for(int i=0; i<models.size(); i++) {
      //   std::cout << "model: " << models[i]->GetName() << std::endl;
      // }
      // std::cout << "ROBOT >>>>>>>>>> " << this->robot->GetName() << std::endl;


      // Get the Joint controller
      std::string joint_name = "";
      if (_sdf->HasElement("joint")) joint_name = _sdf->Get<std::string>("joint");
      this->joint = _parent->GetJoint(joint_name);
      this->controller = _parent->GetJointController();
      // this->controller->SetPositionPID(joint_name, common::PID(0.1, 0.0, 0.001, 0, 0, -1.0, 0));

      // Read the keyframes from the json file 
      if (_sdf->HasElement("uri")) {
      
        std::string actions_uri = _sdf->GetElement("uri")->Get<std::string>();
        std::ifstream poses_json;
        poses_json.open(actions_uri);

        Json::Reader reader;
        Json::Value completeJsonData;
        reader.parse(poses_json, completeJsonData);

        std::cout << "Setting up:" << actions_uri << std::endl;
        #ifdef DEBUG
        std::cout << "Complete JSON data:" << std::endl << completeJsonData << std::endl;
        #endif

        for (int i = 0; i < completeJsonData["keyframes"].size(); i++){
          
          // Build the action/motion structure for later 
          float time = completeJsonData["keyframes"][i]["time"].asFloat();
          double pose = completeJsonData["keyframes"][i]["pose"].asFloat();

          #ifdef DEBUG
            std::cout << "time: " << time << std::endl;
            std::cout << "pose: " << time << std::endl;
          #endif

          Pose pose_struct = {.pose = pose, .time = time};
          this->poses_keyframes.push_back(pose_struct);
        }
        poses_json.close();

        this->pose_to_execute = this->poses_keyframes[0];
      }

      // Select the join from the profile

      // Listen to the update event. This event is broadcast every simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&DynamicJointModelPlugin::OnUpdate, this, std::placeholders::_1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo &_info)
    {
      // Read current sim time      
      float current_time = physics::get_world()->SimTime().Float();
      // std::cout << "current time: " << current_time << "\t pose:" << this->curr_pose << std::endl;

      // If the current sim time reaches next keyframe, then update velocity
      // std::cout << "ROBOT >>>>>>>>>> " << this->robot->GetName() << std::endl;
      if ( current_time >= this->pose_to_execute.time && this->poses_keyframes.size() > 0){
        std::vector<physics::ModelPtr> models = this->world->Models();
        for(int i=0; i<models.size(); i++) {
          std::cout << "model: " << models[i]->GetName() << std::endl;
        }
        this->curr_pose = this->pose_to_execute.pose;
        this->poses_keyframes.erase(this->poses_keyframes.begin());
        this->pose_to_execute = this->poses_keyframes[0];

        std::cout << this->joint->GetName() << std::endl;
        std::cout << "keyframe reached: " << this->pose_to_execute.time << "\t pose:" << this->curr_pose << std::endl;
        
        this->controller->SetJointPosition(this->joint, this->curr_pose);
      }

      
      // Apply current pose
    }

    struct Pose {
      double pose;
      float time;
    };

    // Pointer to the model
    private: physics::ModelPtr model;
    private: sdf::ElementPtr sdf;
    private: std::vector<Pose> poses_keyframes;
    private: gazebo::physics::JointControllerPtr controller;
    private: gazebo::physics::JointPtr joint;
    private: physics::ModelPtr robot;
    private: physics::WorldPtr world;
    
    private: double curr_pose;
    private: Pose pose_to_execute;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(DynamicJointModelPlugin)
}