/*!
 * @file
 *
 * @brief �T�E���h�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <cri_le_atom_ex.h>
#include <string>
#include <list>
#include <vector>
#include <functional>

/**
 *@brief �T�E���h�Ǘ��N���X
 */
class  SoundManager
{
public :
    /**
     * @brief �������p�����[�^
     */
    struct InitializeParam {
        int max_bgm;
        int max_se;
        int max_voice;
        int max_sampling_rate;
        float frame_rate;
        const char* acf_file;
    };
    
public:
    /**
     * @brief ���������s���܂��B
     *
     * @param param �������p�����[�^���w�肵�܂��B
     */
    void Initialize(const InitializeParam& param);
    
    /**
     * @brief �I�������s���܂��B
     */
    void Finalize();
    
    /**
     * @brief BGM�t�@�C����ǂݍ��݂܂��B
     *
     * @param acb_file ACB�t�@�C���̃p�X���w�肵�܂��B
     * @param awb_file AWB�t�@�C���̃p�X���w�肵�܂��B
	 *
	 * @return ����ȏꍇ�� true ��Ԃ��܂��B
     */
    bool LoadBgmFile(const std::string& acb_file, const std::string& awb_file);

    /**
     * @brief SE�t�@�C����ǂݍ��݂܂��B
     *
     * @param acb_file ACB�t�@�C���̃p�X���w�肵�܂��B
	 *
	 * @return ����ȏꍇ�� true ��Ԃ��܂��B
	 */
    bool LoadSeFile(const std::string& acb_file);
    
    /**
     * @brief BGM���Đ����܂��B
     *
	 * @param id id���w�肵�܂��B
     * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     * @param fadein_time �t�F�[�h�C���̎��Ԃ�b�Ŏw�肵�܂��B
     * @param fadeout_time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
	 * @param track �Đ��g���b�N���w�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
	 */
	int PlayBgm(int id, float delay = 0.0f, float fadein_time = 0.5f, float fadeout_time = 0.5f, int track = 0);

    /**
     * @brief BGM���Đ����܂��B
     *
     * @param cue cue�����w�肵�܂��B
	 * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     * @param fadein_time �t�F�[�h�C���̎��Ԃ�b�Ŏw�肵�܂��B
     * @param fadeout_time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
	 * @param track �Đ��g���b�N���w�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
	 */
    int PlayBgm(const std::string& cue, float delay = 0.0f, float fadein_time = 0.5f, float fadeout_time = 0.5f, int track = 0);

    /**
     * @brief BGM���~���܂��B
     *
	 * @param time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
	 * @param track �Đ��g���b�N���w�肵�܂��B
	 */
    void StopBgm(float time = 0.5f, int track = 0);

    /**
     * @brief BGM�̃{�����[����ݒ肵�܂��B
     *
     * @param volume 0.0�`1.0�Ŏw�肵�܂��B
     */
	void SetVolumeBgm(float volume);

    /**
     * @brief BGM�̃{�����[�����擾���܂��B
     *
     * @return �{�����[����Ԃ��܂��B
     */
    float GetVolumeBgm() const { return bgm_volume_; }

    /**
     * @brief BGM�̒�~�̗L�����擾���܂��B
     *
	 * @param track �Đ��g���b�N���w�肵�܂��B
	 *
	 * @return ��~���̏ꍇ�� true ��Ԃ��܂��B
     */
    bool IsStopBgm(int track = 0) const;
    
	/**
	 * @brief BGM���ꎞ��~���܂��B
	 */
	void PauseBgm(int track = 0);

	/**
	* @brief BGM���ĊJ���܂��B
	*/
	void ResumeBgm(int track = 0);

	/**
     * @brief SE���Đ����܂��B
     *
     * @param id id���w�肵�܂��B
     * @param track �Đ��g���b�N���w�肵�܂��B-1���w�肳�ꂽ�ꍇ�A�󂢂Ă���g���b�N���g���čĐ����s���܂��B
     * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
     */
    int PlaySe(int id, int track = -1, float delay = 0.0f);
    
    /**
     * @brief SE���Đ����܂��B
     *
     * @param cue cue�����w�肵�܂��B
     * @param track �Đ��g���b�N���w�肵�܂��B-1���w�肳�ꂽ�ꍇ�A�󂢂Ă���g���b�N���g���čĐ����s���܂��B
     * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
     */
    int PlaySe(const std::string& cue, int track = -1, float delay = 0.0f);
    
    /**
     * @brief SE���~���܂��B
     *
	 * @param track ��~����g���b�N�ԍ����w�肵�܂��B-1�̏ꍇ�͑S�Ă�SE���~���܂��B
	 * @param time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
     */
    void StopSe(int track = -1, float time = 0.5f);
    
    /**
     * @brief SE�̃{�����[����ݒ肵�܂��B
     *
     * @param volume 0.0�`1.0�Ŏw�肵�܂��B
     */
    void SetVolumeSe(float volume);
    
    /**
     * @brief SE�̃{�����[�����擾���܂��B
     *
     * @return �{�����[����Ԃ��܂��B
     */
    float GetVolumeSe() const { return se_volume_; }
    
    /**
     * @brief SE�̒�~�̗L�����擾���܂��B
     *
     * @return ��~���̏ꍇ�� true ��Ԃ��܂��B
     */
    bool IsStopSe(int track) const;

    /**
     * @brief �{�C�X���Đ����܂��B
     *
     * @param file �t�@�C�������w�肵�܂��B
     * @param id id���w�肵�܂��B
     * @param track �Đ��g���b�N���w�肵�܂��B-1���w�肳�ꂽ�ꍇ�A�󂢂Ă���g���b�N���g���čĐ����s���܂��B
     * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
     */
    int PlayVoice(const std::string& file, int id, int track = -1, float delay = 0.0f);
    
    /**
     * @brief �{�C�X���Đ����܂��B
     *
     * @param file �t�@�C�������w�肵�܂��B
     * @param cue cue�����w�肵�܂��B
     * @param track �Đ��g���b�N���w�肵�܂��B-1���w�肳�ꂽ�ꍇ�A�󂢂Ă���g���b�N���g���čĐ����s���܂��B
     * @param delay �Đ��J�n�܂ł̑҂����Ԃ�b�Ŏw�肵�܂��B
     *
     * @return �Đ������g���b�N��Ԃ��܂��B�Đ��ł��Ȃ������ꍇ��-1��Ԃ��܂��B
     */
    int PlayVoice(const std::string& file, const std::string& cue, int track = -1, float delay = 0.0f);

    /**
     * @brief �{�C�X���~���܂��B
     *
     * @param time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
     */
    void StopVoice(int track = -1, float time = 0.5f);

    /**
     * @brief �{�C�X�̃{�����[����ݒ肵�܂��B
     *
     * @param volume 0.0�`1.0�Ŏw�肵�܂��B
     */
    void SetVolumeVoice(float volume);

    /**
     * @brief �{�C�X�̃{�����[�����擾���܂��B
     *
     * @return �{�����[����Ԃ��܂��B
     */
    float GetVolumeVoice() const { return voice_volume_; }

    /**
     * @brief �{�C�X�̒�~�̗L�����擾���܂��B
     *
     * @return ��~���̏ꍇ�� true ��Ԃ��܂��B
     */
    bool IsStopVoice(int track) const;
    
    /**
     * @brief �T�E���h�̍X�V�������s���܂��B
     *        ���̃��\�b�h�͖��t���[���Ăяo�����s���Ă��������B
     */
    void Update(float df);

    /**
     * @brief �S�T�E���h�̒�~���s���܂��B
     *
     * @param time �t�F�[�h�A�E�g�̎��Ԃ�b�Ŏw�肵�܂��B
     */
    void StopAll(float time = 0.0f);
    
    static SoundManager* GetInstance() {
        static SoundManager v;
        return &v;
    }
private :
	class SoundType {
	public :
		enum Type {
			BGM,
			SE,
			VOICE
		};
	};
    struct AcbInfo {
        std::string file;
        CriAtomExAcbHn data;
    };
    
	struct PlayerInfo {
		CriAtomExPlayerHn player;
		float wait_time;
		std::function<int()> func;
	};

    SoundManager();
	~SoundManager();
    
    int GetTrackIndex_(SoundType::Type type, int index) const;
    void ReleaseAcb_(CriAtomExAcbHn& handle);
    void ReleaseAcbList_(std::list<AcbInfo>& handles);
    void ReleaseVoicePool_(CriAtomExVoicePoolHn& handle);
	CriAtomExAcbHn GetAcbData_(std::list<AcbInfo>& acb_infos, const std::string& file, int max_value, bool is_awb);
    
    int PlayBgm_(std::function<void(CriAtomExPlayerHn, CriAtomExAcbHn)> acb_func, float delay, float fadein_time, float fadeout_time, int track);
    int PlaySe_(std::function<void(CriAtomExPlayerHn)> acb_func, int track, float delay);
    int PlayVoice_(std::function<void(CriAtomExPlayerHn, CriAtomExAcbHn)> acb_func, const std::string& file, int track, float delay);

    InitializeParam param_;
    bool initialized_;
    
    CriAtomExAcbHn base_bgm_data_;
    CriAtomExAcbHn base_se_data_;
    std::list<AcbInfo> voice_list_;
    std::vector<PlayerInfo> player_infos_;
    std::string last_bgm_name_;
    int last_bgm_id_;
    
    float bgm_volume_;
    float se_volume_;
    float voice_volume_;
    
    CriAtomExVoicePoolHn hca_pool_;
    CriAtomExVoicePoolHn hca_mx_pool_;
    CriAtomDbasId dbas_id_;
};
