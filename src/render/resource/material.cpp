#include "pch.h"

#include "material.h"


material::manager_t material::manager;


void material::Enable()
{
    auto pShader = mShader.lock();
    if (!pShader) {
        return;
    }

    pShader->Enable();

    /* Apply texture uniforms */
    int sampler = 0;
    for (auto& pair : textures) {
        auto pTex = pair.second.lock();
        if (pTex) {
            pTex->SetSampler(sampler++);
            pTex->Enable();
            pShader->SetUniform(pair.first, pTex->GetSampler());
        }
    }

    /* Apply uniforms */
    for (const auto& it : unifFloat) {
        pShader->SetUniform(it.first, it.second);
    }
    for (const auto& it : unifFloat2) {
        pShader->SetUniform(it.first, it.second);
    }
    for (const auto& it : unifFloat4) {
        pShader->SetUniform(it.first, it.second);
    }
    for (const auto& it : unifInt2) {
        pShader->SetUniform(it.first, it.second);
    }
}


void material::Disable()
{
    auto pShader = mShader.lock();
    if (!pShader) {
        return;
    }

    int sampler = 0;
    for (auto& pair : textures) {
        auto pTex = pair.second.lock();
        if (pTex) {
            pTex->SetSampler(sampler++);
            pTex->Disable();
            pShader->SetUniform(pair.first, pTex->GetSampler());
        }
    }

    pShader->Disable();
}
