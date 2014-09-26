'''
A set of classes related to the generation of signals.
'''
import math

class Signal(object):
    
    def __init__(self, period, amplitude=None, v_range=(0.0, 1.0), offset=0.0):
        self._period = float(period)
        self._v_min = v_range[0]
        self._v_max = v_range[1]
        self.offset = offset
        
        if amplitude != None:
            v_max = self.v_min + amplitude
         
    def next():
        pass
        
    def prev():
        pass
        
    @property
    def period(self):
        return self._period
    
    @period.setter
    def period(self, period):
        self._period = period
        
    @property
    def amplitude(self):
        return self.v_max - self.v_min
        
    @amplitude.setter
    def amplitude(self, amplitude):
        self.v_max = self.v_min + amplitude
        
    @property
    def v_max(self):
        return self._v_max
        
    @property
    def v_min(self):
        return self._v_min
        
    @property
    def v_range(self):
        return (self.v_min, self.v_max)
        
#class ListSignal(UserList):
#    pass
        
class SineWave(Signal):
    
    def __init__(self, period, amplitude=None, v_range=(0.0, 1.0), offset=0.0):
        super(SineWave, self).__init__(period, amplitude, v_range, offset) #TODO: Verify this
        self._t = 0.0
        self.__last_t = 0.0
        self.t_adjust = 0.0 # This is how much to adjust the t value that we get from the user
        self.offset = offset #TODO: deal with this.  
    
    @property
    def period(self): return super(SineWave, self).period
     
    @period.setter
    def period(self, period):
        self._t = self._t * period / self.period
        # We want (last_t + t_adjust) % period = _t % period
        self.t_adjust = self.__last_t - self._t #TODO: does this work?
        self._period = float(period)
        
    def value(self, t, norm=False):
        self._t = (t + self.t_adjust) % self.period
        self.__last_t = t
        v = (math.sin((self.t+self.offset) * 2 * math.pi / self.period) + 1.0) * self.amplitude / 2.0 + self.v_min
        if norm:
            if v < 0.0: return 0.0
            elif v > 1.0: return 1.0
        
        return v
        
    @property
    def t(self):
        return self._t % self.period
        
class SignalGroup(Signal):
    # SignalGroup assumes that incoming signals range in (-1.0, 1.0) and adjusts the range based on that fact.

    def __init__(self, signals, amplitude=1.0, offset=0.0, in_range=(-1.0, 1.0), v_range=(0.0, 1.0)):
        super(SignalGroup, self).__init__(0, amplitude, v_range, offset)
        
        self.signals = signals
        self._in_range = in_range
        
        self._period = reduce( lambda a, b: a*b, map(lambda signal: signal.period, signals))
        
    def value(self, t, norm=True):
        val = reduce(lambda a, b: a*b, map(lambda signal: signal.value(t+self.offset), self.signals))
        v = (val - self._in_range[0]) / (self._in_range[1] - self._in_range[0]) * self.amplitude + self.v_min
        
        if norm:
            if v < 0.0: return 0.0
            elif v > 1.0: return 1.0
        
        return v
