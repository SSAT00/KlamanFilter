#include "integrator.h"
#include <QtMath>
#include <QVector>
#include <QDebug>

QVector<QVector<long double> > Integrator::getXGrid() const
{
    return xGrid;
}

void Integrator::setXGrid(const QVector<QVector<long double> > &value)
{
    xGrid = value;
}

QVector<QVector<long double> > Integrator::getYGrid() const
{
    return yGrid;
}

void Integrator::setYGrid(const QVector<QVector<long double> > &value)
{
    yGrid = value;
}

Integrator::Integrator() {

}

Integrator::~Integrator()
{

}

void Integrator::init(const int imode,
                      const int nxy,
                      const int ns,
                      const long double etol,
                      const long double step,
                      const QVector<long double> x,
                      const QVector<long double> y)
{
    //режим работы
    setIMODE(imode);

    //настройки интегратора
    setNxy(nxy);
    setNs(ns);
    setEtolErr(etol);
    setNi(3);
    setStep(step);

    //начальные условия для интегрирования
    setX(x);
    setY(y);
}



void Integrator::lobbie(QVector<long double> timeGrid) {

    //Определить начало и конец
    long double ts = 0, tf;

    //параметры точности
    long double etol[nxy];

    for (int i=0; i<nxy; i++){
        etol[i] = etolErr;
    }


    //инициализация массивов
    QVector<QVector<long double>> a(ns,QVector<long double> (nxy));
    long double b[ns][ns];
    long double d[ns][ns];
    long double e[ns][ns];
    long double ax[ns][nxy];
    long double da[ns][nxy];

    QVector<long double> c(ns);
    long double g[ns+1];
    QVector<long double> us(nxy), u(nxy),v(nxy);
    QVector<long double> f(nxy), fg(nxy);
    long double fx[nxy];
    int ietol[nxy];

    int nr, ni0=25, cc, it, l, ncf0, netol;
    long double sgm=3.16, eps=1e-14,eta, ecal, prd, dnxt;
    long double t_, t, h, h2, deg, rmn, r, rmx;
    bool ls,vs;
    int IOBS;       // итератор по измерениям

    ts = 0.0;
    if (timeGrid[timeGrid.size() - 1] > 0.0){
        tf = timeGrid[timeGrid.size() - 1];         //при прямом интегрировании
        IOBS = 0;
    }
    else {
        tf = timeGrid[0];
        IOBS = timeGrid.size() - 1 ;                //при обратном интегрировании
    }


    // Spacing
    nr = ns/2-1;
    l = ((ns - 1) / 2 - 1) * (ns / 2 - 1);

    c[0] = 0.0;
    c[ns-1] = 1.0;
    if (ns>2)
    {
        c[nr+1]= 0.5;

        for (int i = 1; i<=nr ; i++) {
            c[i] = Integrator::LobbieSpacing[l+i-1];
        }
        for (int i = 1 ;i<=nr ; i++){
            c[ns-i-1] = 1.0 - Integrator::LobbieSpacing[l+i-1];
        }
    }


     // Node Differences
    for (int i=0; i<ns; i++){
        for (int j=0; j<ns; j++){
            d[i][j] = c[i] - c[j];
        }
    }

    // Constants
    for (int i=0; i<ns; i++){
        prd = 1.0;

        for (int k=0; k<=ns; k++){
            prd = prd*c[i]/static_cast<long double>(k+1);
            g[k] = prd;
        }
        b[i][0] = g[0];
        e[i][0] = g[1];

        for (int j = 1; j<ns; j++){
            for (int k = 0; k<ns-j+1; k++){
                g[k] = d[i][j-1]*g[k] - static_cast<long double>(k+1)*g[k+1];
            }
            b[i][j] = g[0];
            e[i][j] = g[1];
        }
    }

    //Initialization
    for (int i=0; i<ns; i++){
        for (int j=0; j<nxy; j++){
            a[i][j] = 0.0;
            ax[i][j] = 0.0;
            da[i][j] = 0.0;
        }
    }

    for (int i=0; i<nxy; i++){
        us[i] = 0.0;
    }

    t = ts;
    h = abs(step) * (tf - ts) / abs(tf - ts);
    r = 1.0;
    ncf = 1;
    ncf0 = ni0*(ns-1);
    nst = 0;
    if (ni<=0) ni = ni0;

    // Variable Step-Size Mode
    netol = 0;
    for (int i=0; i<nxy; i++){
        if (etol[i] != 0.0) {
            netol = netol + 1;
            ietol[netol-1] = i;
        }
    }

    vs = netol !=0;
    deg = 1.0 / static_cast<long double>(ns);
    rmx = pow(sgm,deg);
    rmn = 1.0 / rmx;

//
    perturbations.forse(nxy,x, y, t, f);
//
    for (int i=0; i<nxy; i++){
        a[0][i] = f[i];
    }

    //Start Step-Size
    if (h==0.0) {
        if (!vs) {
            qDebug() << "Zero Step!";
            //return;
        }
        eta = sqrt(eps);
        ecal = 0.0;

        while (ecal==0.0){
            eta = eta*10.0;
            for (int i = 0; i<nxy; i++){
                u[i] = x[i] + eta*y[i] + eta*eta*a[0][i]/2.0;
                v[i] = y[i] + eta*a[0][i];
            }

            t_ = t + eta;

            perturbations.forse(nxy,u,v, t_, fg); ncf += 1;

            for (int i = 0; i<netol; i++){
                int j = ietol[i];
                ecal = ecal + ((fg[j]-a[0][j])/etol[j])*((fg[j]-a[0][j])/etol[j]) ;
            }
        }
        h = sqrt(2.0 * eta / sqrt(ecal));
        h = h * (tf - ts) / abs(tf - ts);
    }

    //mark_1:    //метка 1
    h = r*h;
    h2 = h*h;      //1 метка выбора шага
    ls = false;

    //First & Last Step
    if ((tf-t)/h<1.0){
        h = tf - t;
        h2 = h*h;
        ls = true;
    }

    while (true){
        mark_2:   //метка 2
        for ( it=0; it<ni; it++){

            for (int i=1; i<ns; i++){

                //Collocation Solution
                for (int k = 0; k<nxy; k++){
                    fx[k]=0.0;
                }

                for (int j = 0; j<ns; j++){
                    for (int k = 0; k<nxy; k++){
                        fx[k] = fx[k] + e[i][j] * a[j][k];
                    }
                }

                for (int j = 0; j<nxy; j++){
                    u[j]= x[j] + h*c[i]*y[j] + h2*fx[j];
                }

                for (int k = 0; k<nxy; k++){
                    fg[k]=0.0;
                }

                for (int j = 0; j<ns; j++){
                    for (int k=0; k<nxy; k++){
                        fg[k] = fg[k] +  b[i][j]*a[j][k];
                    }
                }


                for (int k=0; k<nxy; k++){
                    v[k]=y[k] + h*fg[k];
                }

                //Function
                t_ = t + h*c[i];

                perturbations.forse(nxy,u,v, t_, fg); ncf += 1;

                //Devided Difference
                for (int j=0; j<i; j++){
                    for (int k=0; k<nxy; k++){
                        fg[k]= (fg[k] - a[j][k]) / d[i][j];
                    }
                }

                for (int k=0; k<nxy; k++){
                    a[i][k]= fg[k];
                }

            }

            // Convergence Check
            cc = 0;
            for (int k=0; k<nxy; k++){
                if (abs(u[k]-us[k])<abs(eps*u[k])) cc+=1;  //проверка на сходимость для всех уравнений
            }
            if (cc==nxy) break;      //выскочить из цикла при сходимости
            us = u;
        }

        //Forced convergence
        if ((nst==0) && (ncf <ncf0) && (it>ni)) goto mark_2;

        //Scale Factor
        if (vs) {
            ecal = 0.0;

            for (int i=0; i<netol; i++){
                int j = ietol[i];
                ecal = ecal + (a[ns-1][j]/etol[j])*(a[ns-1][j]/etol[j]);
            }

            if (ecal != 0.0) {
                ecal = abs(h)*sqrt(ecal)*deg;
                r = pow(1.0/ecal,deg);

            }
            else {
                r= rmx;
            }

            // First Step-Size Adjustment
            //if (nst == 0) {
               //if ((rmx<r)&&ls) goto mark_3;
               //if (rmx<r || r<rmn) goto mark_1;
               //}
            if (r>rmx) r = rmx; //Damping
            if (r<rmn) {
                ls = false;
                dnxt = 0.0;
                goto mark_4;
            }
        }

        //Solution
        //mark_3:  //метка 3

        //------- Вывод промежуточных решений -------------------------
        IMODE = 1;
        if (IMODE == 1) {
            output_forecast(timeGrid, t, h, h2, c, a, x, y, IOBS);
        }

        //------- Вывод промежуточных решений -------------------------


        for (int k=0; k<nxy; k++){
            fx[k]=0.0;
        }

        for (int j=0; j<ns; j++){
            for (int k=0; k<nxy; k++){
                fx[k] = fx[k] + e[ns-1][j]*a[j][k];
            }
        }

        for (int k=0; k<nxy; k++){
            x[k]= x[k] + h*y[k] + h2*fx[k];
        }

        for (int k=0; k<nxy; k++ ){
            fg[k] = 0.0;
        }

        for (int j=0; j<ns; j++){
            for (int k=0; k<nxy; k++ ){
                fg[k] = fg[k] + b[ns-1][j]*a[j][k];
            }
        }

        for (int k=0; k<nxy; k++ ){
            y[k]= y[k] + h*fg[k];
        }

        t += h;
        nst +=1;
        dnxt = 1.0;

        if (ls) break;

        mark_4:     //метка 4

        h = r*h;
        h2 = h*h;
        for (int k = 0; k<nxy; k++){
            us[k] = 0.0;
        }
        // Last Step
        if ((tf-t)/h<1.0) {
            r=r*(tf-t)/h;
            step=abs(h);
            h=tf-t;
            h2=h*h;
            ls=true;
        }

        //Extrapolation
        for (int i=0; i<ns; i++){
            for (int k=0; k<nxy; k++){
                fg[k]=0.0;
            }
            for (int j = ns-1; j>=0; j--){
                for (int k=0; k<nxy; k++){
                    fg[k]=a[j][k] + (dnxt + r*c[i]-c[j])*fg[k];
                }
            }

            for (int j=0; j<=i-1; j++){
                for (int k=0; k<nxy; k++){
                    fg[k] = (fg[k]-ax[j][k])/d[i][j];
                }
            }

            //Correction
            if (nst!=1) {
                for (int k=0; k<nxy; k++){
                    da[i][k] = a[i][k]-ax[i][k];
                }
            }

            for (int j=0; j<nxy; j++){
                ax[i][j]= fg[j];
            }
        }

        //Predictor
        for (int k=0; k<ns; k++){
            for (int k1=0; k1<nxy; k1++) {
                a[k][k1] = ax[k][k1] + dnxt*da[k][k1];
            }
        }

    }

    //очищение
    us.clear(); u.clear(); v.clear(); fg.clear(); f.clear(),a.clear(); c.clear();

}

void Integrator::output_forecast(QVector<long double> timeGrid,
                                 long double t, long double h, long double h2,
                                 QVector<long double> c,
                                 QVector<QVector<long double> > a_,
                                 QVector<long double> x,
                                 QVector<long double> y,
                                 int& IOBS)
{
    ephemeris_t curEphem;
    long double s, t_obs, prd;
    long double g[ns+1], bi[ns], ei[ns];
    long double fx[nxy], f[nxy];
    QVector<long double> xout(nxy), yout(nxy);

    for(int i = 0; i < 45; i++) curEphem.derivatives_x.append(0.0L);
    for(int i = 0; i < 45; i++) curEphem.derivatives_y.append(0.0L);

    // -- начало цикла
    while(true) {

        //проверка наличия измерения
        if (IOBS<0 || IOBS>=timeGrid.size()) break;

        //Момент времени измерения   
        t_obs = timeGrid[IOBS];

        //условие на околонулевое значение времени (стартовое)
        if (abs(t_obs)<1e-10) t_obs =0.0;

        // попадание времени измерения в интервал между t и t+h
        if ((t-t_obs)*(t+h-t_obs) > 0.0) break;

        // Dimensionless Grid Time
        s = (t_obs - t)/h;

        // Coefficients
        prd = 1.0;

        for (int k=0; k<=ns; k++){
            prd = prd*s/static_cast<long double>(k+1);
            g[k]= prd;
        }

        bi[0] = g[0];
        ei[0] = g[1];

        for (int j=1; j<ns; j++ ){
            for (int k=0; k<ns-j+1; k++){
                g[k]= (s - c[j-1])*g[k] - static_cast<long double>(k+1)*g[k+1];
            }
            bi[j] = g[0];
            ei[j] = g[1];
        }

        // Solution at Grid Time
        for (int i=0; i<nxy; i++){
            fx[i] = 0.0;
        }

        for (int j=0; j<ns; j++){
            for (int i=0; i<nxy; i++){
                fx[i]= fx[i] + ei[j]*a_[j][i];
            }
        }
        for (int i=0; i<nxy; i++){
            xout[i] = x[i] + h*s*y[i] + h2*fx[i];   // Coordinates
        }

        // обнуление f
        for (int i=0; i<nxy; i++){
            f[i]=0.0;
        }

        for (int j=0; j<ns; j++){
            for (int i=0; i<nxy; i++){
                f[i] = f[i] + bi[j]*a_[j][i];
            }
        }
        for (int i=0; i<nxy; i++){
            yout[i] = y[i] + h*f[i];                // Velocities
        }

        if (h>0.0) {
            // при прямом интегрировании
            IOBS += 1;
        }
        else {
            // при обратном интегрировании
            IOBS -= 1;
        }

        //Запись интегрируемых величин и их производных (если есть)
        xGrid.append(xout);
        yGrid.append(yout);

        //запись эфемерид
        curEphem.pos[0] = xout[0];
        curEphem.pos[1] = xout[1];
        curEphem.pos[2] = xout[2];
        curEphem.pos[3] = yout[0];
        curEphem.pos[4] = yout[1];
        curEphem.pos[5] = yout[2];
        curEphem.time = t_obs;
        for(int i = 0; i < 45; i++) curEphem.derivatives_x[i] = xout[i + 3];
        for(int i = 0; i < 45; i++) curEphem.derivatives_y[i] = yout[i + 3];
        ephemeris.append(curEphem);


        //проверка выхода за границы массива измерений
        if (IOBS<0 || IOBS>=timeGrid.size()) return;

    }
    // -- конец цикла
}

void Integrator::setX(const QVector<long double> &value)
{
    x = value;
}

void Integrator::setY(const QVector<long double> &value)
{
    y = value;
}


void Integrator::integratorClear()
{
    x.clear();
    y.clear();
    xGrid.clear();
    yGrid.clear();
    ephemeris.clear();
}

void Integrator::ephemerisClear()
{
    ephemeris.clear();
}

void Integrator::xyClear()
{
    x.clear();
    y.clear();
}

void Integrator::xyGridClear()
{
    xGrid.clear();
    yGrid.clear();
}

int Integrator::getNcf() const
{
    return ncf;
}

int Integrator::getNst() const
{
    return nst;
}

long double Integrator::getStep() const
{
    return step;
}

long double Integrator::getEtolErr() const
{
    return etolErr;
}

int Integrator::getNi() const
{
    return ni;
}

int Integrator::getNs() const
{
    return ns;
}

int Integrator::getNxy() const
{
    return nxy;
}

void Integrator::setIMODE(int value)
{
    IMODE = value;
}

int Integrator::getIMODE() const
{
    return IMODE;
}

void Integrator::setNxy(int value)
{
    nxy = value;
}

void Integrator::setStep(long double value)
{
    step = value;
}

void Integrator::setEtolErr(long double value)
{
    etolErr = value;
}

void Integrator::setNi(int value)
{
    ni = value;
}

void Integrator::setNs(int value)
{
    ns = value;
}

QVector<long double> Integrator::getX() const
{
    return x;
}

QVector<long double> Integrator::getY() const
{
    return y;
}

QVector<ephemeris_t> Integrator::getEphemeris() const {
    return ephemeris;
}

const long double Integrator::LobbieSpacing[] {
    0.27639320225002103035908263312687,
    0.17267316464601142810085377187657,
    0.11747233803526765357449851302033,
    0.35738424175967745184292450297956,
    0.08488805186071653506398389301626,
    0.26557560326464289309811405904562,
    0.06412992574519669233127711938966,
    0.20414990928342884892774463430102,
    0.39535039104876056561567136982732,
    0.05012100229426992134382737779083,
    0.16140686024463112327705728645432,
    0.31844126808691092064462396564567,
    0.04023304591677059308553366958883,
    0.13061306744724746249844691257008,
    0.26103752509477775216941245363437,
    0.41736052116680648768689011702091,
    0.03299928479597043283386293195030,
    0.10775826316842779068879109194577,
    0.21738233650189749676451801526112,
    0.35212093220653030428404424222047,
    0.02755036388855888829620993084839,
    0.09036033917799666082567920914154,
    0.18356192348406966116879757277817,
    0.30023452951732553386782510421652,
    0.43172353357253622256796907213015,
    0.02334507667891804405154726762227,
    0.07682621767406384156703719645062,
    0.15690576545912128696362048021682,
    0.25854508945433189912653138318153,
    0.37535653494688000371566314981289,
    0.02003247736636954932244991899228,
    0.06609947308482637449988989854586,
    0.13556570045433692970766379973955,
    0.22468029853567647234168864707046,
    0.32863799332864357747804829817916,
    0.44183406555814806617061164513192,
    0.01737703674808071360207430396519,
    0.05745897788851185058729918425888,
    0.11824015502409239964794076201185,
    0.19687339726507714443823503068163,
    0.28968097264316375953905153063071,
    0.39232302231810288088716027686354,
    0.01521597686489103352387863081627,
    0.05039973345326395350268586924007,
    0.10399585406909246803445586451842,
    0.17380564855875345526605839017970,
    0.25697028905643119410905460707656,
    0.35008476554961839595082327263885,
    0.44933686323902527607848349747704,
    0.01343391168429084292151024906313,
    0.04456000204221320218809874680113,
    0.09215187438911484644662472338123,
    0.15448550968615764730254032131377,
    0.22930730033494923043813329624797,
    0.31391278321726147904638265963237,
    0.40524401324084130584786849262344
};
