#pragma once
#include <MMDeviceAPI.h>
#include <AudioClient.h>
#include <AudioPolicy.h>
#include "ResamplerUtil.h"

//
//  WASAPI Capture class.
class CWASAPICapture {
public:
    /// Initializes an instance of CWASAPICapture type.
    CWASAPICapture(IMMDevice *Endpoint);

    /// Uninitialize an instance of CWASAPICapture type.
    /// <remarks>
    /// Shuts down the capture code and frees all the resources.
    /// </remarks>
	~CWASAPICapture(void);

    /// Initialize the capturer.
    /// <param name="EngineLatency">
    /// Number of milliseconds of acceptable lag between live sound being produced and recording operation.
    /// </param>
    /// <returns>
    /// true if capturer was initialized successfully, false otherwise.
    /// </returns>  
    bool Initialize(UINT32 EngineLatency);

    ///  Start capturing audio data.
    /// <param name="waveFile">
    /// [in] Handle to wave file where audio data will be written.
    /// </param>
    /// <returns>
    /// true if capturer has successfully started capturing audio data, false otherwise.
    /// </returns>
    bool Start(HANDLE waveFile);

    /// Stop the capturer.
    void Stop();

    /// Get format of audio written to file.
    /// <returns>
    /// WAVEFORMATEX representing audio format.
    /// </returns>
    WAVEFORMATEX *GetOutputFormat() { return &_OutFormat; }

    /// Get number of bytes of audio data captured so far into wave file.
    /// <returns>
    /// Number of bytes of audio data captured so far into wave file.
    /// </returns>
    DWORD BytesCaptured() { return _BytesCaptured; }

private:
    //
    //  Core Audio Capture member variables.
    //
    IMMDevice *             _Endpoint;
    IAudioClient *          _AudioClient;
    IAudioCaptureClient *   _CaptureClient;
    IMFTransform *          _Resampler;

    HANDLE                  _CaptureThread;
    HANDLE                  _CaptureFile;
    HANDLE                  _ShutdownEvent;
    LONG                    _EngineLatencyInMS;
    WAVEFORMATEX *          _MixFormat;
    DWORD                   _MixFrameSize;
    WAVEFORMATEX            _OutFormat; 

    //
    //  Capture buffer member variables
    //
    size_t                  _InputBufferSize;
    IMFMediaBuffer *        _InputBuffer;
    IMFSample *             _InputSample;
    size_t                  _OutputBufferSize;
    IMFMediaBuffer *        _OutputBuffer;
    IMFSample *             _OutputSample;
    DWORD                   _BytesCaptured;
    
    /// Capture thread - captures audio from WASAPI, processes it with a resampler and writes it to file.
    /// <param name="Context">
    /// [in] Thread data, representing an instance of CWASAPICapture type.
    /// </param>
    /// <returns>
    /// Thread return value.
    /// </returns>
    static DWORD __stdcall WASAPICaptureThread(LPVOID Context);

    /// Capture thread - captures audio from WASAPI, processes it with a resampler and writes it to file.
    /// <returns>
    /// Thread return value.
    /// </returns>
    DWORD DoCaptureThread();

    /// Take audio data captured from WASAPI and feed it as input to audio resampler.
    /// <param name="pBuffer">
    /// [in] Buffer holding audio data from WASAPI.
    /// </param>
    /// <param name="bufferSize">
    /// [in] Number of bytes available in pBuffer.
    /// </param>
    /// <param name="flags">
    /// [in] Flags returned from WASAPI capture.
    /// </param>
    /// <returns>
    /// S_OK on success, otherwise failure code.
    /// </returns>
    HRESULT ProcessResamplerInput(BYTE *pBuffer, DWORD bufferSize, DWORD flags);

    /// Get data output from audio resampler and write it to file.
    /// <param name="pBytesWritten">
    /// [out] On success, will receive number of bytes written to file.
    /// </param>
    /// <returns>
    /// S_OK on success, otherwise failure code.
    /// </returns>
    HRESULT ProcessResamplerOutput(DWORD *pBytesWritten);

    /// Initialize WASAPI in timer driven mode, and retrieve a capture client for the transport.
    /// <returns>
    /// S_OK on success, otherwise failure code.
    /// </returns>
    bool InitializeAudioEngine();

    /// Retrieve the format we'll use to capture samples.
    ///  We use the Mix format since we're capturing in shared mode.
    /// <returns>
    /// true if format was loaded successfully, false otherwise.
    /// </returns>
    bool LoadFormat();
};
