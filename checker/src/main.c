
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zconf.h>
#include <zlib.h>

typedef struct Buff {
  char *dat;
  ulong len;
} Buff;

typedef struct {
  Buff **buffs;
  float **dists;
  uint num;
} BuffDB;

Buff *makeBuff(char *input) {
  Buff *buff = malloc(sizeof(Buff));
  buff->len = strlen(input) + 1;
  buff->dat = (char *)malloc(buff->len);
  strcpy(buff->dat, input);
  return buff;
}

Buff *makeZBuff(Buff *inputBuff) {
  Buff *buff = malloc(sizeof(Buff));
  // Compress the input data
  buff->len = compressBound(inputBuff->len);
  buff->dat = (char *)malloc(buff->len);
  compress((Bytef *)buff->dat, &buff->len, (const Bytef *)inputBuff->dat,
           inputBuff->len);
  return buff;
}

Buff *catBuffs(Buff *buff1, Buff *buff2) {
  Buff *result = malloc(sizeof(Buff));
  result->len = buff1->len + buff2->len - 1;
  result->dat = malloc(result->len);

  strcpy(result->dat, buff1->dat);
  strcat(result->dat, buff2->dat);

  return result;
}

Buff *maxBuff(Buff *x, Buff *y) {
  if (x->len > y->len)
    return x;
  return y;
}

Buff *minBuff(Buff *x, Buff *y) {
  if (x->len < y->len)
    return x;
  return y;
}

float normCompDist(Buff *x, Buff *y) {
  Buff *Cx = makeZBuff(x);
  Buff *Cy = makeZBuff(y);

  Buff *xy = catBuffs(x, y);
  Buff *Cxy = makeZBuff(xy);

  float ncd = (float)((float)Cxy->len - (float)minBuff(Cx, Cy)->len) /
              (float)maxBuff(Cx, Cy)->len;
  free(xy->dat);
  free(xy);
  return ncd;
}

BuffDB *makeDB(uint num, Buff **buffs) {
  BuffDB *db = malloc(sizeof(BuffDB));
  db->num = num;
  db->buffs = buffs;
  db->dists = malloc(sizeof(float *) * num);

  for (int i = 0; i < num; i++) {
    db->dists[i] = malloc(sizeof(float) * num);
    for (int j = 0; j < num; j++) {
      db->dists[i][j] = normCompDist(buffs[i], buffs[j]);
    }
  }
  for (int i = 0; i < num; i++) {
    for (int j = i + 1; j < num; j++) {
      float avg = (db->dists[i][j] + db->dists[j][i]) / 2.0;
      db->dists[i][j] = avg;
      db->dists[j][i] = avg;
    }
  }
  return db;
}

void writeDB(FILE *fd, BuffDB *db) {
  for (int i = 0; i < db->num; i++) {
    for (int j = 0; j < db->num - 1; j++) {
      fprintf(fd, "%f,", db->dists[i][j]);
    }
    fprintf(fd, "%f\n", db->dists[i][db->num - 1]);
  }
}

int main() {
  Buff *titles[] = {
      makeBuff(
          "Federer Makes Triumphant Return to Tennis Circuit After"
          "Lengthy Injury Layoff\n"
          "Swiss tennis maestro Roger Federer marked his long-awaited comeback "
          "to professional tennis today, captivating fans worldwide with his "
          "graceful finesse on the court. Following a prolonged absence due to "
          "a knee injury, Federer displayed remarkable form at the Monte Carlo "
          "Masters, swiftly dispatching his opponent in straight sets. The "
          "20-time Grand Slam champion's return has sparked anticipation for "
          "his upcoming performances and potential appearances at major "
          "tournaments later this season."),
      makeBuff(
          "Diplomatic Summit Convenes Amid Escalating Tensions in Eastern "
          "Region\n"
          "World leaders gathered today for a high-stakes diplomatic summit "
          "aimed at addressing escalating tensions in the volatile Eastern "
          "region. Against a backdrop of heightened geopolitical uncertainty "
          "and military posturing, delegates from key nations engaged in "
          "intensive negotiations to defuse the crisis and seek a diplomatic "
          "resolution. The summit comes amid growing concerns over the "
          "potential for conflict escalation, underscoring the urgent need for "
          "diplomatic dialogue and multilateral cooperation to preserve "
          "regional stability and security."),
      makeBuff("Historic Victory: Women's Soccer Team Clinches Championship "
               "Title in Thrilling Final Match\n"
               "In a thrilling display of skill and determination, the women's "
               "national soccer team secured a historic championship victory "
               "today, electrifying fans around the globe. The tightly "
               "contested final match showcased the team's unwavering resolve "
               "as they battled against formidable opponents, ultimately "
               "emerging triumphant with a last-minute goal in extra time. The "
               "win not only cements their status as champions but also serves "
               "as a testament to the growing popularity and competitiveness "
               "of women's soccer on the international stage."),
      makeBuff(
          "Central Bank Unveils Stimulus Package to Bolster Economic "
          "Recovery Efforts\n"
          "In a bid to bolster economic recovery efforts, the central bank "
          "announced a comprehensive stimulus package today, signaling a "
          "proactive stance to support businesses and stimulate growth. The "
          "package includes measures such as interest rate cuts, liquidity "
          "injections, and targeted financial assistance programs aimed at "
          "revitalizing key sectors hit hardest by the pandemic-induced "
          "downturn. Analysts view the central bank's intervention as a timely "
          "and necessary step to mitigate downside risks and pave the way for "
          "a robust and sustainable economic rebound in the coming months."),
      makeBuff(
          "Underdog Surges: Young Golfer Clinches First Major Title at "
          "Augusta Masters\n"
          "In a captivating display of talent and tenacity, a young golfer "
          "defied the odds to claim their first-ever major title at the "
          "prestigious Augusta Masters tournament. Overcoming a star-studded "
          "field of seasoned competitors, the underdog golfer delivered a "
          "masterful performance, showcasing nerves of steel and impeccable "
          "shot-making throughout the tournament. Their unexpected triumph has "
          "sent shockwaves through the golfing world, inspiring fans and "
          "fellow athletes alike with a story of determination and "
          "breakthrough on one of the sport's grandest stages."),
      makeBuff(
          "Renewable Energy Investment Surges as Global Commitment to "
          "Sustainability Grows\n"
          "Renewable energy investment surged to record highs today as "
          "governments and businesses worldwide reaffirmed their commitment to "
          "combating climate change and transitioning to sustainable energy "
          "sources. With a growing emphasis on environmental responsibility "
          "and decarbonization efforts, investors poured billions of dollars "
          "into renewable energy projects, propelling the sector to "
          "unprecedented levels of growth and innovation. The surge in "
          "investment underscores a seismic shift towards a greener and more "
          "resilient global economy, signaling a brighter future for both the "
          "planet and investors alike."),
      makeBuff(
          "Landmark Trade Agreement Signed Between Global Powers, Easing "
          "Trade Frictions\n"
          "In a significant breakthrough for international trade relations, "
          "global powers finalized a landmark trade agreement today, marking a "
          "pivotal moment in efforts to ease trade frictions and promote "
          "economic cooperation. The agreement, years in the making, addresses "
          "longstanding disputes and barriers to trade, opening up new avenues "
          "for commerce and investment between participating nations. With "
          "tariffs reduced and market access expanded, the agreement is "
          "expected to foster greater economic integration and provide a boost "
          "to global trade flows, offering a ray of hope amid prevailing "
          "geopolitical uncertainties."),
      makeBuff("Historic Peace Accord Signed, Ending Decades-Long Conflict in "
               "Region\n"
               "In a momentous step towards lasting peace and reconciliation, "
               "conflicting parties inked a historic peace accord today, "
               "bringing an end to decades-long hostilities in the region. The "
               "landmark agreement, brokered through arduous negotiations and "
               "international mediation, paves the way for the resolution of "
               "longstanding grievances and the establishment of a framework "
               "for sustainable peace and stability. With hopes high for a new "
               "era of cooperation and prosperity, the accord represents a "
               "triumph of diplomacy and a beacon of hope for "
               "conflict-affected populations striving for a brighter future."),
      makeBuff("Record-Breaking Run: Sprinter Shatters World Record in "
               "100-Meter Dash\n"
               "In a display of unparalleled speed and athleticism, a sprinter "
               "shattered the long-standing world record in the 100-meter "
               "dash, setting a new benchmark for excellence in track and "
               "field. With lightning-fast acceleration and flawless "
               "technique, the sprinter blazed past the competition, crossing "
               "the finish line in jaw-dropping time. Their remarkable feat "
               "not only cements their status as the fastest human on the "
               "planet but also ignites speculation about the possibility of "
               "even greater achievements on the horizon."),
      makeBuff(
          "Digital Currency Adoption Soars as Central Banks Explore CBDCs\n"
          "The adoption of digital currencies soared to new heights today as "
          "central banks around the world intensified their exploration of "
          "central bank digital currencies (CBDCs). With the rise of "
          "cryptocurrencies and the growing digitization of financial "
          "transactions, central banks are racing to develop and implement "
          "CBDCs as a modern alternative to traditional fiat currencies. The "
          "surge in interest reflects a broader shift towards digitalization "
          "in the global economy, with CBDCs poised to revolutionize the way "
          "money is transacted, managed, and regulated in the digital age."),
      makeBuff(
          "Summit Addresses Global Refugee Crisis, Calls for Enhanced "
          "International Cooperation\n"
          "World leaders convened at a high-level summit today to address the "
          "escalating global refugee crisis and chart a path towards greater "
          "international cooperation and solidarity. Against the backdrop of "
          "unprecedented displacement and humanitarian suffering, the summit "
          "focused on identifying innovative solutions to protect the rights "
          "and dignity of refugees, while also addressing the root causes of "
          "forced migration. With millions of lives hanging in the balance, "
          "leaders pledged to redouble their efforts to provide humanitarian "
          "assistance, foster inclusive societies, and promote peace and "
          "stability in regions affected by conflict and instability."),
      makeBuff("Global Markets Rally as Optimism Surrounds Economic Recovery\n"
               "Global financial markets surged today as investor sentiment "
               "soared on the back of positive economic indicators and upbeat "
               "corporate earnings reports. Buoyed by robust manufacturing "
               "data and signs of accelerating growth in key sectors, major "
               "stock indices climbed to new highs, with tech stocks leading "
               "the charge. Analysts attribute the rally to growing confidence "
               "in the post-pandemic recovery trajectory, fueling hopes for "
               "sustained economic expansion in the months ahead.")};

  BuffDB *db = makeDB(12, titles);

  writeDB(stdout, db);

  for (int i = 0; i < db->num; i++) {
    free(titles[i]->dat);
    free(titles[i]);
  }
  for (int i = 0; i < db->num; i++) {
    free(db->dists[i]);
  }
  free(db->dists);
  free(db);

  return 0;
}
