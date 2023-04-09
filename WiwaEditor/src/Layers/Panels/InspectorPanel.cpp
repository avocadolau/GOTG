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
#include <Wiwa/ecs/components/ParticleEmitter.h>
#include <Wiwa/ecs/components/CollisionBody.h>
#include <Wiwa/ecs/systems/AgentAISystem.h>
#include <Wiwa/ecs/components/game/items/Item.h>

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
		if (type->hash == (size_t)TypeHash::ParticleEmitter) { DrawParticleEmitterComponent(data); } else
		if (type->hash == (size_t)TypeHash::AnimatorComponent) { DrawAnimatorComponent(data); } else
		if (type->hash == (size_t)TypeHash::AgentAI) { DrawAiAgentComponent(data); } else
		if (type->hash == (size_t)TypeHash::Item) { DrawItemComponent(data); }else
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
		else if (type->hash == (size_t)TypeHash::ParticleEmitter)
		{
			DrawParticleEmitterComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::AgentAI)
		{
			DrawAiAgentComponent(data);
		}
		else if (type->hash == (size_t)TypeHash::Item)
		{
			DrawItemComponent(data);
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
		if (ImGui::Button("Save Tags", ImVec2(120, 0)))
		{
			// Save Physics Manager json Data
			py.OnSave();
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
				return;
			}
		}

		ImGui::EndDragDropTarget();
	}
	if (animator->animator == nullptr)
		return;

	// get animaitons
	ImGui::Separator();
	const char* animationBaseItems[25];

	for (unsigned int i = 0; i < animator->animator->m_Animations.size(); i++)
	{
		animationBaseItems[i] = animator->animator->m_Animations[i]->m_Name.c_str();
	}
	animationBaseItems[animator->animator->m_Animations.size()] = "none";

	const char* current_item = NULL;
	if (animator->animator->GetCurrentAnimation() != nullptr)
		current_item = animator->animator->GetCurrentAnimation()->m_Name.c_str();
	ImGui::Text("Base animation");
	if (ImGui::BeginCombo("base", current_item))
	{
		for (int n = 0; n < animator->animator->m_Animations.size(); n++)
		{
			bool is_selected = (current_item == animationBaseItems[n]);
			if (ImGui::Selectable(animationBaseItems[n], is_selected))
			{
				current_item = animationBaseItems[n];
				ImGui::SetItemDefaultFocus();
				if (current_item == "none")
				{
					animator->animator->SetCurrentAnimation(nullptr);
				}
				else {

					animator->animator->SetCurrentAnimation(animator->animator->m_Animations[n]);
					animator->animator->GetCurrentAnimation()->m_Loop = animator->Loop;
				}
				
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	const char* animationLayerItems[25];

	for (unsigned int i = 0; i < animator->animator->m_Animations.size(); i++)
	{
		animationLayerItems[i] = animator->animator->m_Animations[i]->m_Name.c_str();
	}

	animationLayerItems[animator->animator->m_Animations.size()] = "none";
	
	const char* current_item2 = NULL;

	if (animator->animator->GetTargetAnimation() != nullptr)
		current_item2 = animator->animator->GetTargetAnimation()->m_Name.c_str();
	ImGui::Text("Layer animation");
	if (ImGui::BeginCombo("layer", current_item2))
	{
		for (int n = 0; n < animator->animator->m_Animations.size(); n++)
		{
			bool is_selected = (current_item2 == animationLayerItems[n]);
			if (ImGui::Selectable(animationLayerItems[n], is_selected))
			{
				current_item2 = animationLayerItems[n];
				ImGui::SetItemDefaultFocus();
				if (current_item2 == "none")
				{
					animator->animator->SetTargetAnimation(nullptr);
				}
				else {

					animator->animator->SetTargetAnimation(animator->animator->m_Animations[n]);
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();

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
	ImGui::Separator();

	ImGui::Text("frames: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(trunc(animator->animator->GetCurrentAnimation()->GetDuration())).c_str());

	float maxTime = animator->animator->GetCurrentAnimation()->GetDuration()/24;
	ImGui::Text("Base animation time");

	if (ImGui::SliderFloat("base time", &animator->baseAnimTime, 0, maxTime))
	{
		animator->animator->GetCurrentAnimation()->m_CurrentTime = animator->baseAnimTime * animator->animator->GetCurrentAnimation()->m_TicksPerSecond;
	}
	ImGui::Separator();

	if (animator->animator->GetTargetAnimation())
	{
		ImGui::Text("frames: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(trunc(animator->animator->GetTargetAnimation()->GetDuration())).c_str());

		float maxTime = animator->animator->GetTargetAnimation()->GetDuration() / 24;
		ImGui::Text("Layer animation time");

		if (ImGui::SliderFloat("layer time", &animator->layerAnimTime, 0, maxTime))
		{
			animator->animator->GetTargetAnimation()->m_CurrentTime = animator->layerAnimTime * animator->animator->GetTargetAnimation()->m_TicksPerSecond;
		}

	}
	
	ImGui::Separator();
	if (ImGui::Checkbox("Blend", &animator->Blend))
	{
		if(animator->Blend)
			animator->animator->SetAnimationState(Wiwa::AnimationState::Blending);
		else
			animator->animator->SetAnimationState(Wiwa::AnimationState::PausedBlending);
	}
	if (ImGui::Checkbox("Loop Blending", &animator->LoopBlend))
	{
		animator->animator->m_LoopBlend = animator->LoopBlend;
	}
	if(ImGui::SliderFloat("Blend Duration", &animator->blendDuration,0,1))
	{
		animator->animator->SetBlendDuration(animator->blendDuration);
	}
	if (ImGui::SliderFloat("Weight", &animator->weight, 0, 1))
	{
		animator->animator->m_BlendWeight = animator->weight;
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

void ParticleTab()
{
	ImGui::Dummy(ImVec2(38, 0));
	ImGui::SameLine();
}

void InspectorPanel::DrawParticleEmitterComponent(byte *data)
{

	Wiwa::ParticleEmitter *emitter = (Wiwa::ParticleEmitter *)data;

	ImGui::Separator();
	ImGui::Text("Emitter Parameters");
	ImGui::Separator();


	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##isPlaying", &emitter->isPlaying);
	ImGui::SameLine();
	ImGui::Text("Playing");

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##isAnimated", &emitter->isAnimated);
	ImGui::SameLine();
	ImGui::Text("Animated");

	// number of animations
	// number of animations
	if (emitter->isAnimated)
	{
		ParticleTab();

		ImGui::Text("Animation Speed");

		ImGui::Dummy(ImVec2(38, 0));
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		ImGui::DragFloat("##particle_aniamtions_speed", &emitter->particle_animation_speed, 0.05f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
	}

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##activateBillboard", &emitter->activateBillboard);
	ImGui::SameLine();
	ImGui::Text("Billboard");

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Checkbox("##repeat", &emitter->repeat);
	ImGui::SameLine();
	ImGui::Text("Loop Spawning");

	// spawn rate
	if (emitter->repeat)
	{
		ParticleTab();
		ImGui::Checkbox("##particle_rate_isRanged", &emitter->particle_rate_isRanged);
		ImGui::SameLine();

		if (emitter->particle_rate_isRanged)
		{
			ImGui::Text("Spawn Rate Range");

			ParticleTab();
			ImGui::PushItemWidth(46.0f);

			ParticleTab();

			ImGui::DragFloat("##particle_rate_range0", &emitter->particle_rate_range_0, 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::DragFloat("##particle_rate_range1", &emitter->particle_rate_range_1, 0.01f, 0.0f, 0.0f, "%.2f");
		}
		else
		{
			ImGui::Text("Spawn Rate");

			ParticleTab();

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);

			ImGui::DragFloat("##particle_rate", &emitter->particle_rate, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopItemWidth();

		if (emitter->particle_rate < 0)
			emitter->particle_rate = 0.01f;
		if (emitter->particle_rate_range_0 < 0)
			emitter->particle_rate_range_0 = 0.01f;
		if (emitter->particle_rate_range_1 < 0)
			emitter->particle_rate_range_1 = 0.01f;
	}

	// spawn amount
	{
		ImGui::Checkbox("##particle_amount_isRanged", &emitter->particle_amount_isRanged);
		ImGui::SameLine();

		if (emitter->particle_amount_isRanged)
		{
			ImGui::Text("Spawn Amount Range");

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();

			ImGui::PushItemWidth(46.0f);

			ImGui::DragInt("##particle_amount_range0", &emitter->particle_amount_range_0, 0.05f, 0.0f, 0.0f, "%.2f");

			ImGui::SameLine();

			ImGui::DragInt("##particle_amount_range1", &emitter->particle_amount_range_1, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{
			ImGui::Text("Spawn Amount");

			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);

			ImGui::DragInt("##particle_amount", &emitter->particle_amount, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopItemWidth();

		if (emitter->particle_amount < 0)
			emitter->particle_amount = 0;
		if (emitter->particle_amount_range_0 < 0)
			emitter->particle_amount_range_0 = 0;
		if (emitter->particle_amount_range_1 < 0)
			emitter->particle_amount_range_1 = 0;
	}

	// particle attributes

	ImGui::Separator();
	ImGui::Text("Particle attributes");
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 4));

	/*ImGui::Checkbox("##particle_followOnSpawn", &emitter->followOnSpawn);
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(2.5, 0));
	ImGui::Text("Follow On Spawn");

	ImGui::Dummy(ImVec2(0, 4));*/


	//particle follow on spawn
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();

		ImGui::Checkbox("##particle_followOnSpawn", &emitter->followOnSpawn);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2.5, 0));
		ImGui::SameLine();
		ImGui::Text("Follow On Spawn");
	}

	//particle follow emitter position
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();

		ImGui::Checkbox("##particle_followEmitterPosition", &emitter->particle_followEmitterPosition);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2.5, 0));
		ImGui::SameLine();
		ImGui::Text("Follow Emitter Position");
	}

	//particle follow emitter rotation
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();

		ImGui::Checkbox("##particle_followEmitterRotation", &emitter->particle_followEmitterRotation);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2.5, 0));
		ImGui::SameLine();
		ImGui::Text("Follow Emitter Rotation");
	}

	ImGui::Dummy(ImVec2(0, 4));

	ImGui::Text("Range");

	ImGui::Dummy(ImVec2(0, 4));

	// particle lifetime
	{
		ImGui::Dummy(ImVec2(0, 0));
		ImGui::SameLine();

		ImGui::Checkbox("##particle_lifetime_isRanged", &emitter->particle_lifetime_isRanged);
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(2, 0));
		ImGui::SameLine();
		ImGui::SameLine();
		ImGui::Text("Particle life time");

		if (emitter->particle_lifetime_isRanged)
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(46.0f);

			ImGui::DragFloat("##particle_lifetime_range0", &emitter->particle_lifetime_range_0, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			ImGui::DragFloat("##particle_lifetime_range1", &emitter->particle_lifetime_range_1, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		else
		{
			ImGui::Dummy(ImVec2(38, 0));
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);
			ImGui::DragFloat("##particle_lifetime", &emitter->particle_lifetime, 0.05f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopItemWidth();

		if (emitter->particle_lifetime < 0)
			emitter->particle_lifetime = 0;
		if (emitter->particle_amount_range_0 < 0)
			emitter->particle_amount_range_0 = 0;
		if (emitter->particle_lifetime_range_1 < 0)
			emitter->particle_lifetime_range_1 = 0;
	}

	if (ImGui::TreeNode("Position & Translation"))
	{
		// particle offset
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_offset_isRanged", &emitter->particle_Offset_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::Text("Particle Offset");

			if (emitter->particle_Offset_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_Offset", &(emitter->particle_Offset)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
		}

		// particle starting position
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingPosition_isRanged", &emitter->particle_startingPosition_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::Text("Particle Starting Position");

			if (emitter->particle_startingPosition_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition_range0", &(emitter->particle_startingPosition_range_0)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition_range1", &(emitter->particle_startingPosition_range_1)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingPosition", &(emitter->particle_startingPosition)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
		}

		// particle initial velocity
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_velocity_isRanged", &emitter->particle_velocity_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Velocity");
			if (emitter->particle_velocity_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity_range0", &(emitter->particle_velocity_range_0)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*	ImGui::SameLine();
					ImGui::Text("Particle Velocity");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity_range1", &(emitter->particle_velocity_range_1)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_velocity", &(emitter->particle_velocity)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Velocity");*/
			}
		}

		// particle constant acceleration
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_acceleration_isRanged", &emitter->particle_acceleration_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Acceleration");
			if (emitter->particle_acceleration_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration_range0", &(emitter->particle_acceleration_range_0)[0], 0.01f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Acceleration");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration_range1", &(emitter->particle_acceleration_range_1)[0], 0.01f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_acceleration", &(emitter->particle_acceleration)[0], 0.01f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Acceleration");*/
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Rotation"))
	{
		// particle starting rotation
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingRotation_isRanged", &emitter->particle_startingRotation_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::Text("Particle Rotation");

			if (emitter->particle_startingRotation_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation_range0", &(emitter->particle_startingRotation_range_0)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation_range1", &(emitter->particle_startingRotation_range_1)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingRotation", &(emitter->particle_startingRotation)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Size & Growth"))
	{
		// particle starting size
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_startingSize_isRanged", &emitter->particle_startingSize_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Starting Size");
			if (emitter->particle_startingSize_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize_range0", &(emitter->particle_startingSize_range_0)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize_range1", &(emitter->particle_startingSize_range_1)[0], 0.05f, 1.0f, 1.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_startingSize", &emitter->particle_startingSize[0], 0.05f, 0.0f, 10.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
			}
		}

		// particle size constant growth rate
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_growthVelocity_isRanged", &emitter->particle_growthVelocity_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Text("Particle Growth Rate");
			if (emitter->particle_growthVelocity_isRanged)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity_range0", &(emitter->particle_growthVelocity_range_0)[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity_range1", &(emitter->particle_growthVelocity_range_1)[0], 0.05f, 0.0f, 0.0f, "%.2f");
			}
			else
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::DragFloat3("##particle_growthVelocity", &emitter->particle_growthVelocity[0], 0.05f, 0.0f, 0.0f, "%.2f");
				/*ImGui::SameLine();
				ImGui::Text("Particle Direction");*/
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Texture"))
	{
		// particle starting size
		{
			/*ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_texture", &emitter->particle_startingSize_isRanged);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();*/

			ImGui::Text("Particle Texture");

			uint32_t image_id = 0;
			Wiwa::Image* texture = nullptr;
			Wiwa::ResourceId textureResource = -4;

			bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(emitter->texturePath);

			if (importedCorrectly)
			{
				textureResource = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath);
				texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textureResource);
			}


			if (texture != nullptr)
				image_id = texture->GetTextureId();

			ImGui::Image(ImTextureID(image_id), { 128,128 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{

					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png" || p.extension() == ".jpg")
					{
						bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(pathS.c_str());

						/*for (size_t i = 0; i < 128; i++)
						{
							emitter->texturePath[i] = (char)"";
						}*/

						emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(pathS.c_str());

						if (emitter->textureId != WI_INVALID_INDEX)
						{
							std::strncpy(emitter->texturePath, pathS.c_str(), pathS.length());
						}
						else
						{
							WI_CORE_INFO("Error loading Image: [WI_INVALID_INDEX]");
							std::strncpy(emitter->texturePath, "", 1);

						}
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (texture)
				ImGui::Text("[Drop a suitable texture here]");
			else
				ImGui::Text("[Drop a suitable texture to change the current texture]");
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Colour"))
	{
		// particle colour
		{
			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::Checkbox("##particle_colour_isRanged", &emitter->particle_colour_range);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(2, 0));
			ImGui::SameLine();
			ImGui::SameLine();

			ImGui::Text("Particle Colour");
			if (emitter->particle_colour_range)
			{
				ImGui::Dummy(ImVec2(38, 0));
				ImGui::SameLine();
				ImGui::ColorPicker4("Color##4", emitter->colorParticles, ImGuiColorEditFlags_None);
			}
		}

		ImGui::TreePop();
	}
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

void InspectorPanel::DrawItemComponent(byte* data)
{
	Wiwa::Item* item = (Wiwa::Item*)data;
	ImGui::InputText("Name",item->Name, 128);
	ImGui::InputScalar("ItemType", ImGuiDataType_U8,&item->item_type);
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
		ImGui::InputText("Name", &em.GetEntityString(m_CurrentID));
		ImGui::SameLine();
		ImGui::Text("(%i)", m_CurrentID);

		DrawCollisionTags();

		if (ImGui::Button("Delete##entity"))
		{
			m_EntitySet = false;
			em.DestroyEntity(m_CurrentID);
		}

		if (ImGui::CollapsingHeader("Components"))
			DrawComponents(em);

		if (ImGui::CollapsingHeader("Systems"))
			DrawSystems(em);

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
