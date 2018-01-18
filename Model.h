#ifndef MODEL_H
#define MODEL_H

class Model {
public:
	float X;			// celular concentration
	float S = 1;		// substrat
	float V = 1;		// volume
	float C_t = 1;		// toxins
	float u = 1;		// specific velocity
	float F = 1;		// debit
	float S_0 = 1;		// debit substrat concentration
	float Y_xs = 1;		// substrat to celular randament
	float F_v = 1;		// losing debit

	float y = 1;		// const
	float M = 1;		// const
	float u_max = 1;	// const
	float K_i = 1;		// const
	float K_s = 1;		// const
	float K_t = 1;		// const

	float q = 1;		// unknown

	Model(float X) : X(X) {}
	
	update() {
		static float lastV;

		X += u * X - X / V * (V - lastV);
		S += F * S_0 / V - u * X / Y_xs - S / V * (V - lastV);
		C_t += q * pow(S, 1. / 3) - C_t / V * (V - lastV)	
		lastV = V;
		V += F - F_v;
	}
	
	void draw(float t) {

	}
};

#endif	