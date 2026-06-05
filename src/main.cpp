#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(CountdownPauseLayer, PauseLayer) {
	struct Fields {
		CCLabelBMFont* m_countdownLabel = nullptr;
		bool isOnCountdown;
		bool shouldActuallyResume;
		int currentNumber = 3;
	};

	void actuallyCountDown(float dt) {
		auto fields = m_fields.self();
		fields->currentNumber -= 1;
		std::string newText = fmt::format("{}", fields->currentNumber);
		fields->m_countdownLabel->setString(newText.c_str(), true);

		auto animation = CCSequence::create(
			CCScaleTo::create(0.f, 1.2f),
			CCEaseOut::create(CCScaleTo::create(Mod::get()->getSettingValue<double>("time-in-s") / 4.f, 1.f), 2.f),
			nullptr
		);
		fields->m_countdownLabel->runAction(animation);
	}

	void timedUnpause(float dt) {
		auto fields = m_fields.self();
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		fields->isOnCountdown = false;
		fields->shouldActuallyResume = true;
		PauseLayer::onResume(nullptr);
	}

	void customSetup() {
		PauseLayer::customSetup();

		auto fields = m_fields.self();
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		fields->currentNumber = 3;
		fields->m_countdownLabel = CCLabelBMFont::create(fmt::format("{}", fields->currentNumber).c_str(), "goldFont.fnt");

		fields->m_countdownLabel->setPosition({winSize.width / 2.f, winSize.height / 2.f - 1000});
		this->addChild(fields->m_countdownLabel);
	}

	void onResume(CCObject* sender) {
		auto fields = m_fields.self();
		auto enabled = Mod::get()->getSettingValue<bool>("enable-mod");

		if (enabled || fields->shouldActuallyResume) {
			PauseLayer::onResume(sender);
			return;
		}

		if (fields->isOnCountdown) return;

		float totalTime = Mod::get()->getSettingValue<double>("time-in-s");
		float theTimeInBetween = totalTime / 3.f;

		auto animation = CCSequence::create(
			CCScaleTo::create(0.f, 1.2f),
			CCEaseOut::create(CCScaleTo::create(totalTime / 4.f, 1.f), 2.f),
			nullptr
		);

		fields->isOnCountdown = true;
		fields->shouldActuallyResume = false;
		fields->m_countdownLabel->runAction(animation);

		this->setPosition({this->getPositionX(), this->getPositionY() + 1000});
		
		this->schedule(schedule_selector(CountdownPauseLayer::actuallyCountDown), theTimeInBetween, 3, 0);
		this->scheduleOnce(schedule_selector(CountdownPauseLayer::timedUnpause), totalTime);
	}

	void onQuit(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::onQuit(sender);
	}
	void tryQuit(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::tryQuit(sender);
	}
	void onRestart(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::onRestart(sender);
	}
	void onRestartFull(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::onRestartFull(sender);
	}
	void onPracticeMode(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::onPracticeMode(sender);
	}
	void onNormalMode(CCObject* sender) {
		if (!m_fields->isOnCountdown) PauseLayer::onNormalMode(sender);
	}
	/*
	void keyDown(enumKeyCodes key, double timestamp) {
		if (m_fields->isOnCountdown) PauseLayer::keyDown(key, timestamp);
	}
	*/
};