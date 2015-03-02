#pragma once

#include <mfapi.h>
#include <mfidl.h>

/// Create Media Foundation transform that resamples audio in specified input format
/// into specified output format.
/// <param name="pwfxIn">
/// [in] Wave format input to resampling operation.
/// </param>
/// <param name="pwfxOut">
/// [in] Wave format output from resampling operation.
/// </param>
/// <param name="ppResampler">
/// [out] Media transform object that will resample audio
/// </param>
/// <returns>
/// S_OK on success, otherwise failure code.
/// </returns>
HRESULT CreateResampler(const WAVEFORMATEX* pwfxIn, const WAVEFORMATEX* pwfxOut, IMFTransform **ppResampler);

/// Create a media buffer to be used as input or output for resampler.
/// <param name="bufferSize">
/// [in] Size of buffer to create.
/// </param>
/// <param name="ppSample">
/// [out] Media Foundation sample created.
/// </param>
/// <param name="ppBuffer">
/// [out] Media buffer created.
/// </param>
/// <returns>
/// S_OK on success, otherwise failure code.
/// </returns>
HRESULT CreateResamplerBuffer(size_t bufferSize, IMFSample** ppSample, IMFMediaBuffer** ppBuffer);