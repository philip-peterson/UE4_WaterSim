// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulatedWater.h"
#include "ImageWriter.h"



bool UImageWriter::WriteCanvasToCsv(UCanvasRenderTarget2D* canvas) {
	return true;


	FArchive *FileWriter = IFileManager::Get().CreateFileWriter(TEXT("C:/Users/ironmagma/Desktop/ue4_output.csv"));
	if ( FileWriter == nullptr ) {
		return false;
	}
	
	bool success = true;

	UTexture2D* tempTex = canvas->ConstructTexture2D(canvas->GetOuter(), FString(TEXT("TempTex")), RF_Transient, CTF_DeferCompression | ~CTF_AllowMips | CTF_SRGB, nullptr);
	// PixelFormat is PF_FloatRGBA

	FTexture2DMipMap mip = tempTex->PlatformData->Mips[0];

	const void* bd = mip.BulkData.LockReadOnly();
	FFloat16 *d = (FFloat16*)bd;

	for ( int32 i = 0; i < mip.SizeY; i++ ) {
		for ( int32 j = 0; j < mip.SizeX; j++ ) {
			FString text;
			text+=FString::SanitizeFloat(*d);
			text+=",";
			text+=FString::SanitizeFloat(*( d + 1 ));
			text+=",";
			text+=FString::SanitizeFloat(*( d + 2 ));
			d+=4;
			text+="\r\n";
			FileWriter->Serialize(TCHAR_TO_ANSI(*text), text.Len());
			if ( FileWriter->GetError() ) {
				success = false;
				break;
			}
		}
		FString text2;
		text2 += "\r\n";
		FileWriter->Serialize(TCHAR_TO_ANSI(*text2), text2.Len());
		if ( FileWriter->GetError() ) {
			success = false;
		}
		if ( !success ) {
			break;
		}
	}

	mip.BulkData.Unlock();




	FileWriter->Close();
	if ( FileWriter->GetError() ) {
		success = false;
	}
	return success;
}

