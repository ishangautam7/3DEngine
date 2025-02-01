if (ImGui::CollapsingHeader("Camera Settings"))
    ImGui::SliderFloat3("Position##Camera", &camera.cameraPos.x, -10.0f, 10.0f);

if (ImGui::CollapsingHeader("Model Transforms")) {
    ImGui::SliderFloat3("Position##Model", &modelPosition.x, -5.0f, 5.0f);
    ImGui::SliderFloat3("Rotation", &modelRotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat("Scale", &modelScale, 0.1f, 5.0f);
}
