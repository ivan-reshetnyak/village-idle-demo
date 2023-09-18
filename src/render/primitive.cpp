#include "pch.h"

#include "primitive.h"


void primitive::Draw()
{
    shader::SetLayout(pGeometry->GetVertArray());
    auto pMaterial = wMaterial.lock();
    if (pMaterial) {
        pMaterial->Enable();
    }

    Render();

    buffer::Unbind();
    if (pMaterial) {
        pMaterial->Disable();
    }
}


void primitive::Set(std::shared_ptr<geometry> geom)
{
    pGeometry = geom;
    mBuffer.Generate(*geom.get());
}
