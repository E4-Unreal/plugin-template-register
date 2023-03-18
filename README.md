# Plugin Template Register
Unreal Engine Plugin for adding external plugin template with modular feature interface
- Experimental Version: 0.1
- Tested in UE 5.1

![image](https://user-images.githubusercontent.com/59055049/226164483-e0b780eb-091f-4a63-bb76-5d793992f490.png)

## 기능  소개
- 플러그인 생성을 위한 템플릿을 추가
- 여러 모듈이 포함된 플러그인 템플릿 역시 생성 가능
  - 단, 모듈 이름에는 플러그인의 이름이 반드시 포함됨
  - ex) **PLUGIN_NAME**Game, **PLUGIN_NAME**Editor

> 모듈 이름을 자유롭게 설정하는 기능 역시 추가하고 싶지만, `PluginUtils`에서는 **PLUGIN_NAME**만 수정해주기 때문에 현재로서는 어렵다.  
이를 해결하기 위해서는 `PluginUtils`, `PluginBrowser`를 대체할 새로운 플러그인을 만들던가 수정이 필요할 것 같다.

## 사용 방법
1. Templates 폴더에 플러그인 템플릿 폴더를 추가
    - 파일명과 파일 내용 중 **PLUGIN_NAME**으로 작성된 부분이 전부 새로운 플러그인 이름으로 치환된다.
	- API의 경우, **PLUGIN_NAME**_API는 사용가능하지만 **PLUGIN_NAME**GAME_API와 같이 플러그인과 모듈 이름이 다른 경우에는 오류가 발생한다. 만약 **PLUGIN_NAME**이 전부 대문자라면 가능하다.
2. 플러그인 환경 설정 파일에 템플릿 정보 추가
    - Config 폴더 > DefaultPluginTemplateRegister.ini 

## 사용시 참고 코드
PLUGIN_NAMEModule.h
```cpp
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPLUGIN_NAMEModule : public IModuleInterface
{
public:
	//~IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End of IModuleInterface
};

```

DefaultPluginTemplateRegister.ini 
```cpp
[/Script/PluginTemplateRegister.PluginTemplateRegisterSettings]
+PluginTemplates=(Path=(Path="/PluginTemplateRegister/Templates/SingleModulePlugin"),Label=NSLOCTEXT("[/Script/PluginTemplateRegister]", "CF987F504FC1329154EF218E9D083DCC", "Single Module"),Description=NSLOCTEXT("[/Script/PluginTemplateRegister]", "B18C82DA4DB873C230CD32B68EBDED33", "Module\'s name is the same as the name of the plugin"), bIsEnabledByDefault=True)
+PluginTemplates=(Path=(Path="/PluginTemplateRegister/Templates/DualModulePlugin"),Label=NSLOCTEXT("[/Script/PluginTemplateRegister]", "F8ED75D242A8177369CC578321429076", "Dual Module"),Description=NSLOCTEXT("[/Script/PluginTemplateRegister]", "7FD268BB4C4DE09A1658168A14B33EC5", "Module\'s base name is the same as the name of the plugin. Suffixes will be added (Game / Editor)"), bIsEnabledByDefault=True, ModuleSuffixes=("Game", "Editor"))
```

TArray\<FPluginTemplateData\> PluginTemplates
```cpp
USTRUCT()
struct FPluginTemplateData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PluginTemplate, meta = (RelativePath))
	FDirectoryPath Path;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Label;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Description;

	/** If true, the created plugin will be enabled by default without needing to be added to the project file. */
	UPROPERTY(config, EditAnywhere, Category = Plugins)
	bool bIsEnabledByDefault = true;
	
	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	TArray<FString> ModuleSuffixes;
};
```

## 참고 플러그인
- `PluginBrowser`의 외부 템플릿 추가 기능 활용
  - 실제 생성은 `PluginUtils`에서 이루어진다.
- `GameFeatures`에서 플러그인 템플릿을 추가하는 기능만 추출 및 수정