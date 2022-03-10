enum havok_version {
	HK500, HK510, HK550, 
	HK600, HK610, HK650, HK660, 
	HK700, HK710, 
	HK2010_1, HK2010_2,
	HK2011_1, HK2011_2, HK2011_3, 
	HK2012_1, HK2012_2, 
	HK2013,
	HK2014,
	HK2015,
	HK2016,
	HK2017,
	HK2018,
	HK2019,
	HK2020,
	HK2021,
	HKUNKVER
};

enum HkxVersion
{
	HKXDES,
	HKXBB,
	HKXSEK,
	HKXER,
	HKXDS1,
	HKXDS2,
	HKXDS3
};

havok_version determineVersion(int ver)
{
	havok_version version;
	switch(ver)
	{
		default: version = HKUNKVER;
		case 8: 
	}

	return version;
};

/*
switch (versions[0]) {
    case 5: {
      switch (versions[1]) {
      case 0:
        return HK500;
      case 1:
        return HK510;
      case 5:
        return HK550;
      }
      return HKUNKVER;
    }

    case 6: {
      switch (versions[1]) {
      case 0:
        return HK600;
      case 1:
        return HK610;
      case 5:
        return HK650;
      case 6:
        return HK660;
      }
      return HKUNKVER;
    }

    case 7: {
      switch (versions[1]) {
      case 0:
        return HK700;
      case 1:
        return HK710;
      }
      return HKUNKVER;
    }

    case 2010: {
      switch (versions[1]) {
      case 1:
        return HK2010_1;
      case 2:
        return HK2010_2;
      }
      return HKUNKVER;
    }

    case 2011: {
      switch (versions[1]) {
      case 1:
        return HK2011_1;
      case 2:
        return HK2011_2;
      case 3:
        return HK2011_3;
      }
      return HKUNKVER;
    }

    case 2012: {
      switch (versions[1]) {
      case 1:
        return HK2012_1;
      case 2:
        return HK2012_2;
      }
      return HKUNKVER;
    }

    case 2013: {
      switch (versions[1]) {
      case 1:
        return HK2013;
      }
      return HKUNKVER;
    }

    case 2014: {
      switch (versions[1]) {
      case 1:
        return HK2014;
      }
      return HKUNKVER;
    }

    default:
      return HKUNKVER;
    }
  };*/