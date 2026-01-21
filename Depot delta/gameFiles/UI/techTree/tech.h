#pragma once

enum techStatus{
	affordable,
	unaffordable,
	locked,
	purchased
};

class tech {
public:
	tech(int tCost) : cost(tCost) {}
	~tech(){}

	void unlock(int money){
		if (money < cost) { status = unaffordable; }
		else { status = affordable; }
	}

	void onBuy() {
		status = purchased;
		effect();
	}

	virtual void effect() = 0;

private:
	int status = locked;

	int cost;
};