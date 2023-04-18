#include "InspectorPanel.h"

#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/math/Vector2i.h>
#include "../../Utils/EditorUtils.h"

#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/ecs/components/AudioSource.h>
#include <Wiwa/ecs/components/PointLight.h>
#include <Wiwa/ecs/components/SpotLight.h>
#include <Wiwa/ecs/components/DirectionalLight.h>
#include <Wiwa/ecs/components/AnimatorComponent.h>
#include <Wiwa/ecs/components/CollisionBody.h>
#include <Wiwa/ecs/systems/AgentAISystem.h>
#include <Wiwa/ecs/systems/ParticleSystem.h>

bool InspectorPanel::DrawComponent(size_t componentId)
{
	bool ret = true;

	Wiwa::EntityManager &em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	const Type *type = em.GetComponentType(componentId);

	std::string name = type->name;

	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string del_label = "Delete##comp";
		del_label += name.c_str();
		if (!(type->hash == (size_t)TypeHash::Transform3D))
		{
			if (ImGui::Button(del_label.c_str()))
			{
				ret = false;
			}
		}

		byte *data = em.GetComponent(m_CurrentID, componentId, type->size);

		// Custom component interface
		if (type->hash == (size_t)TypeHash::Mesh) {	DrawMeshComponent(data); } else
		if (type->hash == (size_t)TypeHash::Transform3D) { DrawTransform3dComponent(data); } else
		if (type->hash == (size_t)TypeHash::AudioSource) { DrawAudioSourceComponent(data); } else
		if (type->hash == (size_t)TypeHash::PointLight) { DrawPointLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::DirectionalLight) { DrawDirectionalLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::SpotLight) { DrawSpotLightComponent(data); } else
		if (type->hash == (size_t)TypeHash::CollisionBody) { DrawCollisionBodyComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderCube) { DrawColliderCubeComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderSphere) { DrawColliderSpehereComponent(data); } else
		if (type->hash == (size_t)TypeHash::ColliderCylinder) { DrawColliderCylinderComponent(data); } else
		if (type->hash == (size_t)TypeHash::RayCast) { DrawRayCastComponent(data); } else
		if (type->hash == (size_t)TypeHash::AnimatorComponent) { DrawAnimatorComponent(data); } else
		if (type->hash == (size_t)TypeHash::AgentAI) { DrawAiAgentComponent(data); } else
		if (type->hash == (size_t)TypeHash::ParticleEmitter) { DrawParticleSystemComponent(data); } else
		// Basic component interface
		if (type->is_class) {
			const Class* cl = (const Class*)type;

			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data, cl->fields[i]);
			}
		}
		else if (type->hash == (size_t)TypeHash::Transform3D)
		{
			DrawTransform3dComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::AudioSource)
		{
			DrawAudioSourceComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::PointLight)
		{
			DrawPointLightComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::DirectionalLight)
		{
			DrawDirectionalLightComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::SpotLight)
		{
			DrawSpotLightComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::AnimatorComponent)
		{
			DrawAnimatorComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::CollisionBody)
		{
			DrawCollisionBodyComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::ColliderCube)
		{
			DrawColliderCubeComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::ColliderSphere)
		{
			DrawColliderSpehereComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::ColliderCylinder)
		{
			DrawColliderCylinderComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::RayCast)
		{
			DrawRayCastComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::AgentAI)
		{
			DrawAiAgentComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::ParticleEmitter)
		{
			DrawParticleSystemComponent(data);
		}
		else

			// Basic component interface
			if (type->is_class)
			{
				const Class *cl = (const Class *)type;

				for (size_t i = 0; i < cl->fields.size(); i++)
				{
					DrawField(data, cl->fields[i]);
				}
			}
		ImGui::TreePop();
	}

	return ret;
}

void InspectorPanel::DrawField(unsigned char *data, const Field &field)
{
	// Draw class field
	if (field.type->is_class)
	{
		const Class *cl = (const Class *)field.type;

		std::string name = field.name;

		if (ImGui::TreeNodeEx(name.c_str()))
		{
			for (size_t i = 0; i < cl->fields.size(); i++)
			{
				DrawField(data + field.offset, cl->fields[i]);
			}
			ImGui::TreePop();
		}
		return;
	}

	// Draw enum field
	if (field.type->is_enum)
	{
		const Enum *en = (const Enum *)field.type;

		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());

		// TODO: DRAW LIST OF ENUMS TO CHOOSE FROM

		ImGui::PopID();
		return;
	}

	// Draw basic fields
	if (field.type->hash == (size_t)TypeHash::Vector2i)
	{
		ImGui::PushID(field.name.c_str());
		DrawInt2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Float)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputFloat("", (float *)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Vector2f)
	{
		ImGui::PushID(field.name.c_str());
		DrawVec2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Vector3f)
	{
		ImGui::PushID(field.name.c_str());
		DrawVec3Control(field.name.c_str(), data, field, 0.0f);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::UInt64)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int *)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Int32)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int *)(data + field.offset));
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Rect2i)
	{
		ImGui::PushID(field.name.c_str());
		DrawRect2Control(field.name.c_str(), data, field);
		ImGui::PopID();
	}
	else if (field.type->hash == (size_t)TypeHash::Pivot)
	{
		ImGui::Text(field.name.c_str());
		ImGui::PushID(field.name.c_str());
		ImGui::InputInt("", (int *)(data + field.offset));
		ImGui::PopID();
	}
}

void InspectorPanel::DrawCollisionTags()
{
	Wiwa::PhysicsManager &py = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

	if (ImGui::Button("Edit Tags"))
		ImGui::OpenPopup("Edit Tags");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Tags", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int selected = -1;
		for (const auto& [key, value] : py.filterMapStringKey)
		{
			/*std::string tempStr = py.GetFilterTag(n);
			int bits = 1 << n;
			tempStr += "_" + std::to_string(bits);*/
			std::string tagNameWitBits = key;
			tagNameWitBits += "_" + std::to_string(value);
			if (ImGui::Selectable(tagNameWitBits.c_str(), selected == value, ImGuiSelectableFlags_DontClosePopups))
				selected = value;
		}
		/*for (int n = 0; n < py.filterMapStringKey.size(); n++)
		{
			std::string tempStr = py.GetFilterTag(n);
			int bits = 1 << n;
			tempStr += "_" + std::to_string(bits);
			if (ImGui::Selectable(tempStr.c_str(), selected == n, ImGuiSelectableFlags_DontClosePopups))
				selected = n;
		}*/

		static char strBuf[64] = "";
		ImGui::InputText("Write Tag", strBuf, 64, ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			if (!(py.filterMapStringKey.find(strBuf) != py.filterMapStringKey.end()))
			{
				py.AddFilterTag(strBuf);
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Remove", ImVec2(120, 0)))
		{
			// If only index > 0, because we don't want to erase the basic tag
			if (selected > 0)
				py.RemoveFilterTag(selected);
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
	// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
	// stored in the object itself, etc.)
	// const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
	// static int itemCurrentIdx = 0; // Here we store our selection data as an index.
	// const char* comboPreviewValue = py.filterStrings[itemCurrentIdx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
	// if (ImGui::BeginCombo("Collision Tags", comboPreviewValue))
	//{
	//	for (int n = 0; n < py.filterStrings.size(); n++)
	//	{
	//		const bool is_selected = (itemCurrentIdx == n);
	//		if (ImGui::Selectable(py.filterStrings[n].c_str(), is_selected))
	//			itemCurrentIdx = n;

	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}

	//

	//	ImGui::EndCombo();
	//}
}

void InspectorPanel::DrawMeshComponent(byte *data)
{
	Wiwa::Mesh *mesh = (Wiwa::Mesh *)data;

	// Draw meshId field
	ImGui::Text("Model");
	ImGui::PushID("meshId");

	Wiwa::Model *mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
	mod = mod->getModelAt(mesh->modelIndex);

	// DEBUG
	// ImGui::SliderFloat("Animation time: ", &mod->GetParent()->animationTime, 0, mod->GetParent()->GetAnimations()[0]->duration);

	AssetContainer(std::filesystem::path(mod->getModelPath()).stem().string().c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t *path = (const wchar_t *)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".fbx" || p.extension() == ".FBX")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				p.replace_extension();
				std::string src = Wiwa::FileSystem::RemoveFolderFromPath("assets", p.string());
				mesh->meshId = Wiwa::Resources::Load<Wiwa::Model>(src.c_str());
				mesh->modelIndex = 0;
				mesh->drawChildren = true;

				Wiwa::Model *m = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Text("Model path: ");
	ImGui::SameLine();
	ImGui::Text(mod->getModelPath());
	ImGui::Text("Bounding box");
	ImGui::InputFloat3("Max", (float *)glm::value_ptr(mod->boundingBox.getMax()));
	ImGui::InputFloat3("Min", (float *)glm::value_ptr(mod->boundingBox.getMin()));
	static bool showNormals = false;
	if (ImGui::Checkbox("Show normals", &showNormals))
		mod->showNormals = showNormals;
	ImGui::PopID();

	// Draw materialId field
	ImGui::Text("Material");
	Wiwa::Material *mat = Wiwa::Resources::GetResourceById<Wiwa::Material>(mesh->materialId);
	AssetContainer(std::filesystem::path(mat->getMaterialPath()).stem().string().c_str());
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t *path = (const wchar_t *)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".wimaterial")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				mesh->materialId = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::PushID("materialId");
	ImGui::Text("Material at: ");
	ImGui::SameLine();
	ImGui::Text(mat->getMaterialPath());
	ImGui::PopID();
}

void InspectorPanel::DrawTransform3dComponent(byte *data)
{
	Wiwa::Transform3D *transform = (Wiwa::Transform3D *)data;
	DrawVec3Control("Position", &transform->localPosition, 0.0f, 100.0f);
	DrawVec3Control("Rotation", &transform->localRotation, 0.0f, 100.0f);
	DrawVec3Control("Scale", &transform->localScale, 1.0f, 100.0f);
}

void InspectorPanel::DrawAudioSourceComponent(byte *data)
{
	Wiwa::AudioSource *asrc = (Wiwa::AudioSource *)data;

	ImGui::Checkbox("Is default listener", &asrc->isDefaultListener);
	ImGui::InputText("Event", asrc->eventName, sizeof(asrc->eventName));
	ImGui::Checkbox("Play on awake", &asrc->playOnAwake);
}

void InspectorPanel::DrawPointLightComponent(byte *data)
{
	Wiwa::PointLight *lsrc = (Wiwa::PointLight *)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient Intensity", &lsrc->AmbientIntensity, 0.05f, 0.5f, "%.2f");
	ImGui::InputFloat("Diffuse Intensity", &lsrc->DiffuseIntensity, 0.05f, 0.5f, "%.2f");
	ImGui::SliderFloat("Constant", &lsrc->Constant, 0.001f, 1.0f);
	ImGui::SliderFloat("Linear", &lsrc->Linear, 0.001f, 1.0f);
	ImGui::SliderFloat("Exponential", &lsrc->Exp, 0.001f, 1.0f);
}

void InspectorPanel::DrawDirectionalLightComponent(byte *data)
{
	Wiwa::DirectionalLight *lsrc = (Wiwa::DirectionalLight *)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient intensity", &lsrc->AmbientIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::InputFloat("Diffuse intensity", &lsrc->DiffuseIntensity, 0.5f, 1.0f, "%.2f");
}
// TODO: Implement when ready
void InspectorPanel::DrawSpotLightComponent(byte *data)
{
	Wiwa::SpotLight *lsrc = (Wiwa::SpotLight *)data;

	ImGui::ColorEdit3("Color", glm::value_ptr(lsrc->Color));
	ImGui::InputFloat("Ambient Intensity", &lsrc->AmbientIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::InputFloat("Diffuse Intensity", &lsrc->DiffuseIntensity, 0.5f, 1.0f, "%.2f");
	ImGui::SliderFloat("Constant", &lsrc->Constant, 0.001f, 1.0f);
	ImGui::SliderFloat("Linear", &lsrc->Linear, 0.001f, 1.0f);
	ImGui::SliderFloat("Exponential", &lsrc->Exp, 0.001f, 1.0f);
	DrawVec3Control("Direction", &lsrc->Direction);
	ImGui::InputFloat("Cutoff", &lsrc->Cutoff);
}

void InspectorPanel::DrawAnimatorComponent(byte *data)
{
	Wiwa::AnimatorComponent *animator = (Wiwa::AnimatorComponent *)data;

	AssetContainer(animator->filePath);
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".wianimator")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				pathS =	Wiwa::Resources::_assetToLibPath(pathS);
				strcpy(animator->filePath, pathS.c_str());
				animator->animator = Wiwa::Animator::LoadWiAnimator(pathS.c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}
	if (animator->animator == nullptr)
		return;

	// get animaitons
	const char* animationItems[10];
	for (unsigned int i = 0; i < animator->animator->m_Animations.size(); i++)
	{
		animationItems[i] = animator->animator->m_Animations[i]->m_Name.c_str();
	}

	const char* current_item = NULL;
	if (animator->animator->GetCurrentAnimation() != nullptr)
		current_item = animator->animator->GetCurrentAnimation()->m_Name.c_str();
	ImGui::Text("Current animation");
	if (ImGui::BeginCombo("animaiton", current_item))
	{
		for (int n = 0; n < animator->animator->m_Animations.size(); n++)
		{
			bool is_selected = (current_item == animationItems[n]);
			if (ImGui::Selectable(animationItems[n], is_selected))
			{
				current_item = animationItems[n];
				ImGui::SetItemDefaultFocus();
				animator->animator->SetCurrentAnimation(animator->animator->m_Animations[n]);
				animator->animator->PlayAnimation();
				animator->animator->Loop(animator->Loop);
			}
		}
		ImGui::EndCombo();
	}
	if (!animator->animator->GetCurrentAnimation())
		return;
	if (ImGui::Checkbox("Play", &animator->Play))
	{
		if (animator->Play)
			animator->animator->PlayAnimation();
		else
			animator->animator->PauseAnimation();
	}

	if (ImGui::Checkbox("Loop", &animator->Loop))
	{
		animator->animator->Loop(animator->Loop);
	}
	ImGui::Text(std::to_string(animator->animator->GetCurrentAnimation()->GetDuration()).c_str());
	float maxTime =animator->animator->GetCurrentAnimation()->GetDuration()/24;
	ImGui::SliderFloat("Aniamtion time", &animator->animationTime, 0, maxTime);

	ImGui::Checkbox("Blend", &animator->Blend);
	if (animator->Blend)
	{
		if (ImGui::TreeNodeEx("Blending"))
		{
			const char* animationItems[10];
			for (unsigned int i = 0; i < animator->animator->m_Animations.size(); i++)
			{
				animationItems[i] = animator->animator->m_Animations[i]->m_Name.c_str();
			}

			const char* current_item = NULL;
			if (animator->animator->GetTargetAnimation() != nullptr)
				current_item = animator->animator->GetTargetAnimation()->m_Name.c_str();
			ImGui::Text("Blend to animation");
			if (ImGui::BeginCombo("blend anim", current_item))
			{
				for (int n = 0; n < animator->animator->m_Animations.size(); n++)
				{
					bool is_selected = (current_item == animationItems[n]);
					if (ImGui::Selectable(animationItems[n], is_selected))
					{
						current_item = animationItems[n];
						ImGui::SetItemDefaultFocus();
						animator->animator->SetTargetAnimation(animator->animator->m_Animations[n]);
					}
				}
				ImGui::EndCombo();
			}
			if(ImGui::SliderFloat("Weight", &animator->weight, 0, 1))
			{

			}
			if(ImGui::SliderFloat("Blend Duration", &animator->blendDuration, 0, 5))
			{

			}

			if (animator->animator->GetTargetAnimation())
			{
				animator->animator->SetAnimationSatate(Wiwa::AnimationState::Blending);
			}

			ImGui::TreePop();
		}
	}
}

void InspectorPanel::DrawCollisionBodyComponent(byte* data)
{
	Wiwa::PhysicsManager& py = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
	Wiwa::CollisionBody* collisionBody = (Wiwa::CollisionBody*)data;
	DrawVec3Control("Position offset", &collisionBody->positionOffset, 0.0f, 100.0f);
	DrawVec3Control("Scaling offset", &collisionBody->scalingOffset, 0.0f, 100.0f);
	ImGui::Checkbox("Is static?", &collisionBody->isStatic);
	ImGui::Checkbox("Is trigger?", &collisionBody->isTrigger);
	ImGui::Checkbox("Do continuous?", &collisionBody->doContinuousCollision);

	const char* comboPreviewValue = py.GetFilterTag(collisionBody->selfTag);  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("Self Tag", comboPreviewValue))
	{
		for (const auto& [key, value] : py.filterMapStringKey)
		{
			const bool is_selected = (collisionBody->selfTag == value);
			std::string tagNameWitBits = key;
			tagNameWitBits += "_" + std::to_string(value);
			if (ImGui::Selectable(tagNameWitBits.c_str(), is_selected))
				collisionBody->selfTag = value;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		//for (int n = 0; n < py.filterMapStringKey.size(); n++)
		//{
		//	const bool is_selected = (collisionBody->selfTag == n);
		//	if (ImGui::Selectable(py.GetFilterTag(n), is_selected))
		//		collisionBody->selfTag = n;

		//	// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//	if (is_selected)
		//		ImGui::SetItemDefaultFocus();
		//}

		ImGui::EndCombo();
	}

	ImGui::Text("Collide with:");
	for (const auto& [key, value] : py.filterMapStringKey)
	{
		bool local = (collisionBody->filterBits >> value) & 1; //Checking a bit
		ImGui::Checkbox(key.c_str(), &local);
		if (local)
			collisionBody->filterBits |= 1 << value;
		else
			collisionBody->filterBits &= ~(1 << value);
		///*std::string tempStr = py.GetFilterTag(n);
		//int bits = 1 << n;
		//tempStr += "_" + std::to_string(bits);*/
		//std::string tagNameWitBits = key;
		//tagNameWitBits += "_" + std::to_string(value);
		//if (ImGui::Selectable(tagNameWitBits.c_str(), selected == value, ImGuiSelectableFlags_DontClosePopups))
		//	selected = value;
	}

	//ImGui::Text("Collide with:");
	//for (int i = 0; i < py.filterMapStringKey.size(); i++)
	//{
	//	bool local = (collisionBody->filterBits >> i) & 1; //Checking a bit
	//	ImGui::Checkbox(py.GetFilterTag(i), &local);
	//	if (local)
	//		collisionBody->filterBits |= 1 << i;
	//	else
	//		collisionBody->filterBits &= ~(1 << i);
	//}

	// ImGui::Text("Do not collide with:");
	// for (int i = 0; i < py.filterStrings.size(); i++)
	//{
	//	bool local = (collisionBody->filterBits >> i) & 1U; //Checking a bit
	//	ImGui::Checkbox(py.filterStrings[i].c_str(), &local);
	//	if (local)
	//		collisionBody->filterBits |= 1UL << i;
	//	else
	//		collisionBody->filterBits &= ~(1UL << i);
	// }
}

void InspectorPanel::DrawColliderCubeComponent(byte *data)
{
	Wiwa::ColliderCube *colliderCube = (Wiwa::ColliderCube *)data;
	DrawVec3Control("HalfExtents", &colliderCube->halfExtents, 0.0f, 100.0f);
}

void InspectorPanel::DrawColliderSpehereComponent(byte *data)
{
	Wiwa::ColliderSphere *colliderSphere = (Wiwa::ColliderSphere *)data;
	ImGui::InputFloat("Radius", &colliderSphere->radius);
}

void InspectorPanel::DrawColliderCylinderComponent(byte *data)
{
	Wiwa::ColliderCylinder *colliderCylinder = (Wiwa::ColliderCylinder *)data;
	ImGui::InputFloat("Radius", &colliderCylinder->radius);
	ImGui::InputFloat("Height", &colliderCylinder->height);
}

void InspectorPanel::DrawRayCastComponent(byte *data)
{
	Wiwa::RayCast *rayCast = (Wiwa::RayCast *)data;
	DrawVec3Control("Ray From", &rayCast->rayFromWorld, 0.0f, 100.0f);
	DrawVec3Control("Ray To", &rayCast->rayToWorld, 0.0f, 100.0f);
	ImGui::Checkbox("Enabled", &rayCast->doRayCasting);
}


void InspectorPanel::DrawAiAgentComponent(byte* data)
{
	Wiwa::AgentAI* agent = (Wiwa::AgentAI*)data;
	DrawVec3Control("Target", &agent->target, 0.0f, 100.0f);
	ImGui::InputFloat("Speed", &agent->speed);
	ImGui::InputFloat("Angular Speed", &agent->angularSpeed);
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::AgentAISystem* agentSys = em.GetSystem<Wiwa::AgentAISystem>(m_CurrentID);

	if (agent)
	{
		if (ImGui::Button("Create path to") && Wiwa::SceneManager::IsPlaying())
		{
			agentSys->CreatePath(agent->target);
		}

		//if (ImGui::Button("Go to next position"))
		//{
		//	agentSys->GoToNextPosition();
		//}
	}
}

void ParticleTab()
{
	ImGui::Dummy(ImVec2(38, 0));
	ImGui::SameLine();
}

void InspectorPanel::DrawParticleSystemComponent(byte* data)
{
	Wiwa::ParticleEmitterComponent* emitter = (Wiwa::ParticleEmitterComponent*)data;
	Wiwa::EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Separator();
	ImGui::Text("Emitter Parameters");
	ImGui::Separator();


	//set mesh
	ImGui::Text("Particle Mesh Type");
	AssetContainer(emitter->m_meshPath);
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".fbx")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				pathS = Wiwa::Resources::_assetToLibPath(pathS);
				strcpy(emitter->m_meshPath, pathS.c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::Dummy(ImVec2(0, 4));

	//set material
	ImGui::Text("Particle Material");
	AssetContainer(emitter->m_materialPath);
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring ws(path);
			std::string pathS(ws.begin(), ws.end());
			std::filesystem::path p = pathS.c_str();
			if (p.extension() == ".wimaterial")
			{
				WI_INFO("Trying to load payload at path {0}", pathS.c_str());
				pathS = Wiwa::Resources::_assetToLibPath(pathS);
				strcpy(emitter->m_materialPath, pathS.c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::Dummy(ImVec2(0, 4));
	
	std::string activeParticles = "Active Particles: " + std::to_string(emitter->m_activeParticles);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0.5f, 1));
	ImGui::Text(activeParticles.c_str());
	ImGui::PopStyleColor();

	ImGui::Dummy(ImVec2(0, 4));


	ImGui::Checkbox("##m_active", &emitter->m_active);
	ImGui::SameLine();
	ImGui::Text("Active");


	ImGui::Dummy(ImVec2(0, 4));

	

	ImGui::Text("Max Particles");
	ImGui::PushItemWidth(100.0f);
	
	ImGui::DragInt("##m_maxParticles", &emitter->m_maxParticles, 0.05f, 0.0f, 0.0f, "%.2f");

	ImGui::PopItemWidth();



	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##m_loopSpawning", &emitter->m_loopSpawning);
	ImGui::SameLine();
	ImGui::Text("Loop Spawning");


	if (emitter->m_loopSpawning)
	{
		ParticleTab();
		ImGui::Checkbox("##m_p_rangedSpawnRate", &emitter->m_p_rangedSpawnRate);
		ImGui::SameLine();
		ImGui::Text("Spawn Rate");

		if (emitter->m_p_rangedSpawnRate)
		{
			ParticleTab();
			ImGui::PushItemWidth(46.f);

			ImGui::DragFloat("##m_p_minSpawnRate", &emitter->m_p_minSpawnRate, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::DragFloat("##m_p_maxSpawnRate", &emitter->m_p_maxSpawnRate, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);

			ImGui::DragFloat("##m_spawnRate", &emitter->m_spawnRate, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
	}

	ImGui::Dummy(ImVec2(0, 4));



	if (emitter->m_loopSpawning)
	{

		ImGui::Checkbox("##m_rangedSpawnAmount", &emitter->m_rangedSpawnAmount);
		ImGui::SameLine();
		ImGui::Text("Spawn Amount");

		if (emitter->m_rangedSpawnAmount)
		{

			ImGui::PushItemWidth(46.f);

			ImGui::DragInt("##m_minSpawnAmount", &emitter->m_minSpawnAmount, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::DragInt("##m_maxSpawnAmount", &emitter->m_maxSpawnAmount, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);

			ImGui::DragInt("##m_spawnAmount", &emitter->m_spawnAmount, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
	}




	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Separator();
	ImGui::Text("Particle attributes");
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##m_billboardActive", &emitter->m_billboardActive);
	ImGui::SameLine();
	ImGui::Text("Use Billboarding");

	ImGui::Dummy(ImVec2(0, 4));

	//lifetime
	ImGui::Checkbox("##m_p_rangedLifeTime", &emitter->m_p_rangedLifeTime);
	ImGui::SameLine();
	ImGui::Text("Life Time");

	if (emitter->m_p_rangedLifeTime)
	{
		ImGui::PushItemWidth(46.f);

		ImGui::DragFloat("##m_p_minLifeTime", &emitter->m_p_minLifeTime, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();
		ImGui::DragFloat("##m_p_maxLifeTime", &emitter->m_p_maxLifeTime, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(100.0f);
		ImGui::DragFloat("##m_p_lifeTime", &emitter->m_p_lifeTime, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
	}
	ImGui::Dummy(ImVec2(0, 4));

	if (ImGui::TreeNode("Position & Translation"))
	{

		ImGui::Text("Use Volume:");
		ImGui::SameLine();
		const char* volumeItems[] = { "None", "Cube", "Sphere" };
		static const char* volumeCurrentItem = NULL;

		switch (emitter->m_spawnVolume)
		{
		case Wiwa::ParticleSpawnVolume::NONE:
		{
			volumeCurrentItem = "None";
		}
		break;
		case Wiwa::ParticleSpawnVolume::CUBE:
		{
			volumeCurrentItem = "Cube";

		}
		break;
		case Wiwa::ParticleSpawnVolume::SPHERE:
		{
			volumeCurrentItem = "Sphere";

		}
		break;
		default:
		{
			volumeCurrentItem = "None";

		}
		break;
		}


		ImGui::PushItemWidth(100);
		if (ImGui::BeginCombo("##combo", volumeCurrentItem)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(volumeItems); n++)
			{
				bool is_selected = (volumeCurrentItem == volumeItems[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(volumeItems[n], is_selected))
				{
					volumeCurrentItem = volumeItems[n];

					switch (n)
					{
					case Wiwa::ParticleSpawnVolume::NONE:
					{
						emitter->m_spawnVolume = Wiwa::ParticleSpawnVolume::NONE;
					}
					break;
					case Wiwa::ParticleSpawnVolume::CUBE:
					{
						emitter->m_spawnVolume = Wiwa::ParticleSpawnVolume::CUBE;

					}
					break;
					case Wiwa::ParticleSpawnVolume::SPHERE:
					{
						emitter->m_spawnVolume = Wiwa::ParticleSpawnVolume::SPHERE;
					}
					break;

					default:
					{
						emitter->m_spawnVolume = Wiwa::ParticleSpawnVolume::NONE;
					}
						break;
					}
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();


		switch (emitter->m_spawnVolume)
		{
		case Wiwa::ParticleSpawnVolume::NONE:
		{

			ImGui::Text("Initial Position");
			ImGui::DragFloat3("##m_p_initialPosition", &(emitter->m_p_initialPosition)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		break;
		case Wiwa::ParticleSpawnVolume::CUBE:
		{

			ImGui::Text("Spawn Area:");

			ImGui::Text("Corner A");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_InitialPositionBoxA", &(emitter->m_p_InitialPositionBoxA)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Corner B");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_InitialPositionBoxB", &(emitter->m_p_InitialPositionBoxB)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		break;
		case Wiwa::ParticleSpawnVolume::SPHERE:
		{
			ImGui::Text("Center Position");
			ImGui::DragFloat3("##m_p_initialPositionSphCenter", &(emitter->m_p_initialPositionSphCenter)[0], 0.05f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Spawn Radius");
			ImGui::DragFloat("##m_p_initialPositionSphRadius", &emitter->m_p_initialPositionSphRadius, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		break;

		default:
		{

			ImGui::Text("Initial Position");
			ImGui::DragFloat3("##m_p_initialPosition", &(emitter->m_p_initialPosition)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		break;
		}



		
		ImGui::Dummy(ImVec2(0, 8));

		ImGui::Checkbox("##m_p_rangedVelocity", &emitter->m_p_rangedVelocity);
		ImGui::SameLine();
		ImGui::Text("Initial Velocity");


		if (emitter->m_p_rangedVelocity)
		{


			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_minVelocity", &(emitter->m_p_minVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_maxVelocity", &(emitter->m_p_maxVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{

			ImGui::DragFloat3("##m_p_initialVelocity", &(emitter->m_p_initialVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");


		}
		
		ImGui::TreePop();
		ImGui::Dummy(ImVec2(0, 8));
	}

	if (ImGui::TreeNode("Rotation & Angular Velocity"))
	{
		ImGui::Checkbox("##m_p_initialRotation", &emitter->m_p_rangedInitialRotation);
		ImGui::SameLine();
		ImGui::Text("Initial Rotation");

		if (emitter->m_p_rangedInitialRotation)
		{
			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_minInitialRotation", &(emitter->m_p_minInitialRotation)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_maxInitialRotation", &(emitter->m_p_maxInitialRotation)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{

			ImGui::DragFloat3("##m_p_initialRotation", &(emitter->m_p_initialRotation)[0], 0.05f, 0.0f, 0.0f, "%.2f");


		}

		ImGui::Dummy(ImVec2(0, 8));

		ImGui::Checkbox("##m_p_rangedAngularVelocity", &emitter->m_p_rangedAngularVelocity);
		ImGui::SameLine();
		ImGui::Text("Initial Angular Velocity");


		if (emitter->m_p_rangedAngularVelocity)
		{


			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_minAngularVelocity", &(emitter->m_p_minAngularVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_maxAngularVelocity", &(emitter->m_p_maxAngularVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{

			ImGui::DragFloat3("##m_p_initialAngularVelocity", &(emitter->m_p_initialAngularVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");


		}

		ImGui::TreePop();
		ImGui::Dummy(ImVec2(0, 8));
	}

	if (ImGui::TreeNode("Scaling & Growth"))
	{
		ImGui::Checkbox("##m_p_rangedInitialScale", &emitter->m_p_rangedInitialScale);
		ImGui::SameLine();
		ImGui::Text("Initial Scale");

		if (emitter->m_p_rangedInitialScale)
		{
			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_minInitialScale", &(emitter->m_p_minInitialScale)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_maxInitialScale", &(emitter->m_p_maxInitialScale)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{

			ImGui::DragFloat3("##m_p_initialScale", &(emitter->m_p_initialScale)[0], 0.05f, 0.0f, 0.0f, "%.2f");


		}

		ImGui::Dummy(ImVec2(0, 8));

		ImGui::Checkbox("##m_p_rangedGrowthVelocity", &emitter->m_p_rangedGrowthVelocity);
		ImGui::SameLine();
		ImGui::Text("Initial Growth Velocity");


		if (emitter->m_p_rangedGrowthVelocity)
		{


			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_minInitialGrowthVelocity", &(emitter->m_p_minInitialGrowthVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::DragFloat3("##m_p_maxInitialGrowthVelocity", &(emitter->m_p_maxInitialGrowthVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{

			ImGui::DragFloat3("##m_p_initialGrowthVelocity", &(emitter->m_p_initialGrowthVelocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");


		}

		ImGui::TreePop();
		ImGui::Dummy(ImVec2(0, 8));
	}

	if (ImGui::TreeNode("Color"))
	{
		


		if (emitter->m_colorsUsed < 1)
		{
			emitter->m_colorsUsed = 1;
		}

		if (ImGui::Button("Sort Colors by Percentage"))
		{
			for (int i = 0; i < emitter->m_colorsUsed; i++) {
				int minIndex = i;
				for (int j = i + 1; j < emitter->m_colorsUsed; j++) {
					if (emitter->m_p_colorsOverLifetime[j].m_percentage < emitter->m_p_colorsOverLifetime[minIndex].m_percentage) {
						minIndex = j;
					}
				}
				auto temp = emitter->m_p_colorsOverLifetime[i];
				emitter->m_p_colorsOverLifetime[i] = emitter->m_p_colorsOverLifetime[minIndex];
				emitter->m_p_colorsOverLifetime[minIndex] = temp;
			}
		}

		if (ImGui::Button("Reset unused Colors"))
		{
			for (size_t i = emitter->m_colorsUsed; i < 128; i++)
			{
				emitter->m_p_colorsOverLifetime[i].m_percentage = 0;
				emitter->m_p_colorsOverLifetime[i].color = glm::vec4(0,0,0,1);
			}
		}

		

		//todo: add delete color and duplicate color
		ImGui::Dummy(ImVec2(0, 8));
		ImGui::Text("Colors over lifetime:");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::DragInt("##m_colorNodes", &emitter->m_colorsUsed, 0.1f, 1.0f, 128.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("+") && emitter->m_colorsUsed < 128) emitter->m_colorsUsed++;
		ImGui::SameLine();
		if (ImGui::Button("-") && emitter->m_colorsUsed > 1) emitter->m_colorsUsed--;
		ImGui::Dummy(ImVec2(0, 4));

		for (size_t i = 0; i < emitter->m_colorsUsed; i++)
		{
			std::string colorCodePercentage = "##colorCodePer_" + std::to_string(i);
			std::string colorCode = "##colorCode_" + std::to_string(i);

			ImGui::Text("At");
			ImGui::SameLine();
			ImGui::PushItemWidth(75);
			ImGui::DragFloat(colorCodePercentage.c_str(), &emitter->m_p_colorsOverLifetime[i].m_percentage, 0.1f, 0.0f, 100.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("%% use");
			ImGui::SameLine();
			//ImGui::ColorEdit4(colorCode.c_str(), &emitter->m_p_colorsOverLifetime[i].color.x);


			ImVec4 imCol = {
				emitter->m_p_colorsOverLifetime[i].color.x,
				emitter->m_p_colorsOverLifetime[i].color.y,
				emitter->m_p_colorsOverLifetime[i].color.z,
				emitter->m_p_colorsOverLifetime[i].color.w };

			ImVec4 temp = ImVec4(0.2, 0.8, 0.4, 1);

			ImGui::PushStyleColor(ImGuiCol_Button, imCol);

			float bordergrey = 0.75f;
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(bordergrey, bordergrey, bordergrey, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.25));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

			std::string colorCodePickerPopup = "##colorCodePickerPopup_" + std::to_string(i);
			
			if (ImGui::Button(colorCode.c_str(), ImVec2(75, 20)))
			{
				ImGui::OpenPopup(colorCodePickerPopup.c_str());
			}

			// Display a color picker inside a popup
			if (ImGui::BeginPopup(colorCodePickerPopup.c_str()))
			{
				std::string colorCodePicker = "##colorCodePicker_" + std::to_string(i);
				ImGui::ColorPicker4(colorCodePicker.c_str(), (float*)&emitter->m_p_colorsOverLifetime[i].color.x);
				ImGui::EndPopup();
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
			ImGui::SameLine();
			std::string colorCodeAlpha = "##colorCodeAlpha_" + std::to_string(i);
			

			ImGui::Text("A:", i);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::DragFloat(colorCodeAlpha.c_str(), &emitter->m_p_colorsOverLifetime[i].color.w, 0.01f, 0.0f, 1.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::SameLine();
			std::string colorCodeDuplicate = "++##colorCodeDuplicate_" + std::to_string(i);
			if (ImGui::Button(colorCodeDuplicate.c_str()))
			{
				emitter->m_colorsUsed++;
				for (size_t j = emitter->m_colorsUsed; j > i; j--)
				{
					emitter->m_p_colorsOverLifetime[j + 1].m_percentage = emitter->m_p_colorsOverLifetime[j].m_percentage;
					emitter->m_p_colorsOverLifetime[j + 1].color = emitter->m_p_colorsOverLifetime[j].color;
				}
				emitter->m_p_colorsOverLifetime[i + 1].m_percentage = emitter->m_p_colorsOverLifetime[i].m_percentage;
				emitter->m_p_colorsOverLifetime[i + 1].color = emitter->m_p_colorsOverLifetime[i].color;
			}

			ImGui::SameLine();

			std::string colorCodeDelete = "-##colorCodeDelete_" + std::to_string(i);
			if (ImGui::Button(colorCodeDelete.c_str()))
			{
				for (size_t j = i + 1; j < emitter->m_colorsUsed; j++)
				{
					emitter->m_p_colorsOverLifetime[j - 1].m_percentage = emitter->m_p_colorsOverLifetime[j].m_percentage;
					emitter->m_p_colorsOverLifetime[j - 1].color = emitter->m_p_colorsOverLifetime[j].color;
				}
				emitter->m_p_colorsOverLifetime[emitter->m_colorsUsed - 1].m_percentage = 0;
				emitter->m_p_colorsOverLifetime[emitter->m_colorsUsed - 1].color = glm::vec4(0, 0, 0, 1);
				emitter->m_colorsUsed--;

			}

		}
		ImGui::TreePop();
	}


}

InspectorPanel::InspectorPanel(EditorLayer *instance)
	: Panel("Inspector", ICON_FK_INFO_CIRCLE, instance)
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	Wiwa::EntityManager &em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);
	if (m_EntitySet && m_CurrentID >= 0)
	{
		const char *entName = em.GetEntityName(m_CurrentID);
		if (entName)
		{
			std::string edit = entName;

			ImGui::InputText("Name", (char*)edit.c_str(), 64);
			ImGui::SameLine();
			ImGui::Text("(%i)", m_CurrentID);

			DrawCollisionTags();

			if (ImGui::Button("Delete##entity"))
			{
				m_EntitySet = false;
				em.DestroyEntity(m_CurrentID);
			}
			if (strcmp(edit.c_str(), entName) != 0)
				em.SetEntityName(m_CurrentID, edit.c_str());

			if (ImGui::CollapsingHeader("Components"))
				DrawComponents(em);

			if (ImGui::CollapsingHeader("Systems"))
				DrawSystems(em);
		}
	}
	else
	{
		TextCentered("Select an entity to inspect");
	}
	ImGui::End();
}

void InspectorPanel::DrawComponents(Wiwa::EntityManager &em)
{
	std::map<ComponentId, size_t> &map = em.GetEntityComponents(m_CurrentID);
	bool removed = false;
	size_t idToRemove;

	for (std::map<ComponentId, size_t>::iterator comp = map.begin(); comp != map.end(); comp++)
	{
		if (!DrawComponent(comp->first))
		{
			idToRemove = comp->first;
			removed = true;
		}
	}

	if (removed)
	{
		em.RemoveComponentById(m_CurrentID, idToRemove);
	}

	ImGui::Separator();

	if (ButtonCenteredOnLine("Add component"))
		ImGui::OpenPopup("Components");

	size_t c_count = Wiwa::Application::Get().GetComponentTypeCount();

	if (ImGui::BeginPopup("Components"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Search:");
		filter.Draw("##searchbar", 340.f);
		ImGui::BeginChild("listbox child", ImVec2(300, 200));
		for (size_t i = 0; i < c_count; i++)
		{
			const Type *type = Wiwa::Application::Get().GetComponentType(i);
			const char *paintkit = type->name.c_str();
			if (filter.PassFilter(paintkit))
			{
				std::string label = paintkit;

				label += "##" + std::to_string(i);
				if (ImGui::MenuItem(label.c_str()))
				{
					em.AddComponent(m_CurrentID, type);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void InspectorPanel::DrawSystems(Wiwa::EntityManager &em)
{
	std::vector<SystemHash> &systems = em.GetEntitySystemHashes(m_CurrentID);

	for (size_t i = 0; i < systems.size(); i++)
	{
		ImGui::PushID(i);
		const Type *system = Wiwa::Application::Get().GetSystemTypeH(systems[i]);
		ImGui::Text(system->name.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			em.RemoveSystem(m_CurrentID, system->hash);
		}
		ImGui::PopID();
	}

	if (ButtonCenteredOnLine("Add System"))
		ImGui::OpenPopup("System");

	size_t c_count = Wiwa::Application::Get().GetSystemTypeCount();

	if (ImGui::BeginPopup("System"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Search:");
		filter.Draw("##searchbar", 340.f);
		ImGui::BeginChild("listbox child", ImVec2(300, 200));
		for (size_t i = 0; i < c_count; i++)
		{
			const Type *type = Wiwa::Application::Get().GetSystemType(i);
			const char *paintkit = type->name.c_str();
			if (filter.PassFilter(paintkit))
			{
				std::string label = paintkit;

				label += "##" + std::to_string(i);
				if (ImGui::MenuItem(label.c_str()))
				{
					em.ApplySystem(m_CurrentID, type);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void InspectorPanel::Update()
{
}

void InspectorPanel::OnEvent(Wiwa::Event &e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>( {&InspectorPanel::OnEntityChangeEvent, this});
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>( {&InspectorPanel::OnSceneChangeEvent, this});
}

bool InspectorPanel::OnEntityChangeEvent(EntityChangeEvent &e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;
	return false;
}

bool InspectorPanel::OnSceneChangeEvent(Wiwa::SceneChangeEvent &e)
{
	m_CurrentID = -1;
	m_EntitySet = false;
	return false;
}
