#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/ecs/systems/System.h>

class TestSystemAs : public Wiwa::System {
protected:
	void OnAwake() override;

	void OnInit() override;

	void OnUpdate() override;
public:
	TestSystemAs();
	~TestSystemAs();
};

REGISTER_SYSTEM(TestSystemAs);