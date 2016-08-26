<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{4CBD6C0F-AF19-48B5-8093-22651492C39F}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <RootNamespace>project</RootNamespace>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <CharacterSet>NotSet</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>NotSet</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LinkIncremental>true</LinkIncremental>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <PreprocessorDefinitions>_HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0;__WXMSW__;WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>[[cycle|headerPathsDebug|msvcHeaderPath.tpl]]</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <SubSystem>[[projectType]]</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalDependencies>kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;mpfr.lib;[[cycle|librariesDebug|msvcLibrary.tpl]];%(AdditionalDependencies)</AdditionalDependencies>
      <AdditionalLibraryDirectories>[[cycle|libraryPathsDebug|msvcLibraryPath.tpl]]</AdditionalLibraryDirectories>
      <OutputFile>$(ProjectDir)$(TargetName)$(TargetExt)</OutputFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <PreprocessorDefinitions>__WXMSW__;WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>[[cycle|headerPathsRelease|msvcHeaderPath.tpl]]</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <SubSystem>[[projectType]]</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalDependencies>kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;mpfr.lib;[[cycle|librariesRelease|msvcLibrary.tpl]];%(AdditionalDependencies)</AdditionalDependencies>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <AdditionalLibraryDirectories>[[cycle|libraryPathsRelease|msvcLibraryPath.tpl]]</AdditionalLibraryDirectories>
      <OutputFile>$(ProjectDir)$(TargetName)$(TargetExt)</OutputFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <None Include="ReadMe.txt" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="ClassDeclarations.h" />
    <ClInclude Include="ClassSupport.h" />
    <ClInclude Include="ArraySupport.h" />
    <ClInclude Include="HashtableSupport.h" />
    <ClInclude Include="StreamSupport.h" />
    <ClInclude Include="json-forwards.h" />
    <ClInclude Include="json.h" />
    <ClInclude Include="Log.h" />
    <ClInclude Include="LogManager.h" />
    <ClInclude Include="StandardFuncs.h" />
    <ClInclude Include="SymbolEnums.h" />
    <ClInclude Include="targetver.h" />
    <ClInclude Include="UniVar.h" />
    <ClInclude Include="Utils.h" />
    <ClInclude Include="tinyxml2.h" />
    <ClInclude Include="minIni.h" />
    <ClInclude Include="ZMath.h" />
    <ClInclude Include="ZString.h" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="ClassSupport.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="ArraySupport.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="jsoncpp.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="Log.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="LogManager.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="ZMath.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="ZString.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="main.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="StandardFuncs.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="UniVar.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="HashtableSupport.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="StreamSupport.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="GlobalMethods.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="tinyxml2.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="minIni.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="Utils.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
    <ClCompile Include="ArbitraryPrecisionFloat.cpp">
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">NotUsing</PrecompiledHeader>
      <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">NotUsing</PrecompiledHeader>
    </ClCompile>
[[cycle|cppFiles|projectFile.tpl]]
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>