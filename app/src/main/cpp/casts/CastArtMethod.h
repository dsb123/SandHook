//
// Created by 甘尧 on 2019/1/12.
//
#include "../includes/ICast.h"
#include "art/art_6_0.h"

namespace SandHook {

    class CastDexCacheResolvedMethods : public IMember<art::mirror::ArtMethod, void*> {
    protected:
        Size calOffset(art::mirror::ArtMethod p) override {
            if (SDK_INT >= ANDROID_P)
                return getParentSize() + 1;
            auto filter = [](int offset, void *ptr) -> bool {
//                if (isBadReadPtr(ptr, BYTE_POINT))
//                    return false;
//                Size addr = ADDR(ptr);
//                if (isBadReadPtr(reinterpret_cast<void *>(addr + offset), BYTE_POINT))
//                    return false;
//                Size subAddr = ADDR(addr + offset);
//                return addr == subAddr;
            };
            int offset = findOffsetWithCB1(&p, getParentSize(), 4, *filter);
            return offset;
        }
    };

    class CastEntryPointFormInterpreter : public IMember<art::mirror::ArtMethod, void*> {
    protected:
        Size calOffset(art::mirror::ArtMethod p) override {
            if (SDK_INT <= ANDROID_M)
                return getParentSize() - 3 * BYTE_POINT;
            else
                return getParentSize() + 1;
        }
    };

    class CastEntryPointQuickCompiled : public IMember<art::mirror::ArtMethod, void*> {
    protected:
        Size calOffset(art::mirror::ArtMethod p) override {
            return getParentSize() - BYTE_POINT;
        }
    };


    class CastAccessFlag : public IMember<art::mirror::ArtMethod, uint32_t> {
    protected:
        Size calOffset(art::mirror::ArtMethod p) override {
            int offset = findOffset(&p, getParentSize(), sizeof(uint32_t), 524313);
            if (offset < 0)
                throw getParentSize() + 1;
            else
                return static_cast<size_t>(offset);
        }
    };




    class CastArtMethod {
    public:
        static Size size;
        static IMember<art::mirror::ArtMethod, void*>* entryPointQuickCompiled;
        static IMember<art::mirror::ArtMethod, void*>* entryPointFormInterpreter;
        static IMember<art::mirror::ArtMethod, void*>* dexCacheResolvedMethods;
        static IMember<art::mirror::ArtMethod, uint32_t>* accessFlag;

        static void init(JNIEnv *env) {
            //init ArtMethodSize
            jclass sizeTestClass = env->FindClass("com/swift/sandhook/ArtMethodSizeTest");
            Size artMethod1 = (Size) env->GetStaticMethodID(sizeTestClass, "method1", "()V");
            Size artMethod2 = (Size) env->GetStaticMethodID(sizeTestClass, "method2", "()V");
            size = artMethod2 - artMethod1;

            art::mirror::ArtMethod m1 = *reinterpret_cast<art::mirror::ArtMethod *>(artMethod1);
            art::mirror::ArtMethod m2 = *reinterpret_cast<art::mirror::ArtMethod *>(artMethod2);

            //init Members
            entryPointQuickCompiled = new CastEntryPointQuickCompiled();
            entryPointQuickCompiled->init(m1, size);

            accessFlag = new CastAccessFlag();
            accessFlag->init(m1, size);

            entryPointFormInterpreter = new CastEntryPointFormInterpreter();
            entryPointFormInterpreter->init(m1, size);

            dexCacheResolvedMethods = new CastDexCacheResolvedMethods();
//            dexCacheResolvedMethods->init(m1, size);

            //test
            art::mirror::ArtMethod** mArray = reinterpret_cast<art::mirror::ArtMethod**>(m1.dex_cache_resolved_methods_);

            art::mirror::ArtMethod m1B = *(mArray + 8)[m1.dex_method_index_];
            art::mirror::ArtMethod m1C = *(mArray + 8)[m2.dex_method_index_];

        }

        static void copy(art::mirror::ArtMethod* from, art::mirror::ArtMethod* to) {
            memcpy(to, from, size);
        }

    };

    Size CastArtMethod::size = 0;
    IMember<art::mirror::ArtMethod, void*>* CastArtMethod::entryPointQuickCompiled = nullptr;
    IMember<art::mirror::ArtMethod, void*>* CastArtMethod::entryPointFormInterpreter = nullptr;
    IMember<art::mirror::ArtMethod, void*>* CastArtMethod::dexCacheResolvedMethods = nullptr;
    IMember<art::mirror::ArtMethod, uint32_t>* CastArtMethod::accessFlag = nullptr;

}

